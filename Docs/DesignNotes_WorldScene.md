# World / Scene / Entity 설계 노트

## 1. Entity & World

### Entity::m_owner
- `World*` raw pointer 사용 (ObjectPtr 불필요)
- World는 Entity보다 항상 오래 살기 때문에 dangling 위험 없음
- Unreal의 `AActor::GetWorld()` 패턴과 동일한 의도

### 생명주기 및 명시적 Cleanup
- Pool 기반 지연 소멸(deferred purge) 구조에서 소멸자에 World 콜백을 넣으면 안 됨
- 논리적 cleanup은 **명시적 호출**로 처리

```
World::Clear()
    └─ EntityStorage 순회 → entity->Clear()
        └─ m_componentType 순회 → World::RemoveComponent(uuid, typeInfo)
        └─ m_nodeType 순회     → World::RemoveNode(uuid, typeInfo)
    └─ ComponentRegistry.Clear()
    └─ NodeRegistry.Clear()
    └─ EntityStorage.Clear()

World::RemoveEntity(uuid)
    └─ entity->Clear()
    └─ EntityStorage에서 제거
```

- Entity 소멸자는 아무것도 안 함 → Purge는 순수 메모리 반환
- `Entity::Clear()`는 멱등성 보장 (m_componentType이 비면 no-op)

### World::RemoveNode → Scene::Detach 자동 연결
```
World::RemoveNode(uuid, typeInfo)
    → NodeRegistry 제거
    → m_scene.Detach(uuid, typeInfo)   ← Scene 자동 해제
```

---

## 2. ObjectStorage / DataStorage Iterator

### 문제
`Iterator` / `ConstIterator` typedef가 Reflection의 `IteratorTraits`에 매칭되어
`ObjectStorage<T>`가 배열 컨테이너로 인식 → `ValueTraits::ValueType` 없음 → 컴파일 에러

### 해결
```cpp
// 변경 전
using Iterator = typename Data::Iterator;
using ConstIterator = typename Data::ConstIterator;

// 변경 후
using DataIter = typename Data::Iterator;
using DataConstIter = typename Data::ConstIterator;
```

- `begin()` / `end()` 메서드는 유지 → range-for 정상 동작
- Reflection은 내부 `PROPERTY(m_data)` → HashMap 경로로 GC 순회

---

## 3. GENERATE 매크로 - private 상속 지원

private 상속 시 `TypeDetector`가 부모의 `ThisType`에 접근 불가한 문제 해결:

```cpp
#define GENERATE( Class ) \
    public : \
        friend struct Reflection::Utils::TypeDetector<Class>; \  // 추가
        using SuperType = typename Reflection::Utils::TypeDetector<Class>::Type; \
        using ThisType = Class; \
        ...
```

---

## 4. SceneComponent 설계

### 배경 - 기존 구조의 문제
```
ECS::Node (virtual)
├─ wtr::Node<Components...>
└─ SceneNode (virtual)
    └─ TypedSceneNode<Components...>  ← 다이아몬드 상속
        ├─ MeshNode
        └─ LightNode
```
- 다이아몬드 상속 → Reflection 호환 불가
- `TypedSceneNode`에 `m_owner` shadowing 버그 존재

### 변경 방향
- `SceneNode`, `TypedSceneNode` 제거
- `TransformComponent` 제거 → `SceneComponent`로 완전 대체
- `SceneComponent`는 독립 클래스 (상속 없음)

### SceneComponent 구조
```cpp
class SceneComponent : public ECS::Component
{
    GENERATE(SceneComponent);
public:
    void UpdatePosition(const fvec3& pos);   // 값 변경 시 Scene::Update 호출
    void UpdateRotation(const fvec3& rot);
    void UpdateScale(const fvec3& scale);

    const fvec3& GetPosition() const;
    const fvec3& GetRotation() const;
    const fvec3& GetScale()    const;

    void OnAttached(Scene* scene, const ECS::UUID& entityID);  // 첫 Node 등록 시
    void OnDetached();                                          // 마지막 Node 해제 시

private:
    fvec3    m_position;
    fvec3    m_rotation;
    fvec3    m_scale;
    Scene*   m_scene    = nullptr;
    ECS::UUID m_entityID;
};
```

- `Scene*`은 생성 시 주입하지 않고, `OnAttached` 시점에 세팅
- Transform 변경 시 `if (m_scene) m_scene->Update(m_entityID, transform)`

### Node 구조 변경
```cpp
// 제거
class TypedSceneNode<T>

// 변경
class MeshNode   : public wtr::Node<SceneComponent, MeshComponent>
class LightNode  : public wtr::Node<SceneComponent, LightComponent>
class CameraNode : public wtr::Node<SceneComponent, CameraComponent>
```

---

## 5. Scene 설계

### 데이터 구조
```cpp
HashMap<ECS::UUID, Pair<ObjectPtr<SceneComponent>, HashSet<TypeInfo*>>> m_nodes;
// UUID            = entity UUID
// SceneComponent  = transform 추적 + Commander 라우팅
// HashSet<TypeInfo*> = 현재 등록된 Node 타입 목록 (MeshNode, LightNode 등)
```

### Attach
```
Scene::Attach(node)
    → MeshNode?  → HashSet에 MeshNode TypeInfo 추가
                 → 첫 등록이면 SceneComponent::OnAttached(scene, uuid)
                 → Commander::AddPrimitive(mesh, transform)
    → LightNode? → HashSet에 LightNode TypeInfo 추가
                 → 첫 등록이면 SceneComponent::OnAttached(scene, uuid)
                 → Commander::AddLight(light, transform)
```

### Detach
```
Scene::Detach(uuid, typeInfo)
    → HashSet에서 해당 typeInfo 제거
    → Commander::RemovePrimitive(uuid) or Commander::RemoveLight(uuid)
    → HashSet가 비었으면 → SceneComponent::OnDetached() + 엔트리 제거
```

### Transform Update
```
SceneComponent::UpdatePosition(...)
    → m_scene->Update(m_entityID, transform)

Scene::Update(uuid, transform)
    → Commander::UpdateTransform(uuid, transform)
```

### SceneComponent 생명주기
| 시점 | 상태 |
|---|---|
| OnAttached | 첫 번째 Node 타입 등록 시, m_scene 세팅 |
| OnDetached | 마지막 Node 타입 해제 시, m_scene = nullptr |
| m_scene == nullptr | Attach 안 된 상태 → Update 호출해도 no-op |

### 빛나는 큐브 (Mesh + Light 동시 등록) 처리
```
entity->AddNode<MeshNode>()
entity->AddNode<LightNode>()

scene->Attach(entity->GetNode<MeshNode>())   → HashSet: {MeshNode}
scene->Attach(entity->GetNode<LightNode>())  → HashSet: {MeshNode, LightNode}

// MeshNode만 제거
World::RemoveNode(uuid, MeshNode TypeInfo)
    → Scene::Detach(uuid, MeshNode TypeInfo)
    → HashSet: {LightNode}  ← LightNode 유지

// LightNode 제거 시 HashSet 비어서 SceneComponent::OnDetached()
```

---

## 6. World::GetScene() 공개

```cpp
// World.h
public:
    Scene& GetScene() { return m_scene; }
```

외부에서 명시적으로 `scene->Attach()` 호출 가능.
Node 생성(AddNode)과 Scene 등록(Attach)은 분리된 단계.

---

## 7. CameraComponent 명칭 수정

MSVC에서 `near` / `far`는 Win32 예약 매크로.

```cpp
// 변경
float nearPlane = 1.f;
float farPlane  = 1000.f;
```
