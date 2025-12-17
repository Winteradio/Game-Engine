# Custom Game Engine

**바닥부터 구현한 데이터 지향(Data-Oriented) C++ 게임 엔진**

이 프로젝트는 상용 엔진이나 무거운 표준 라이브러리에 의존하지 않고,  
**데이터 지향 설계(DOD)**, **메모리 연속성(Memory Contiguity)**, **로우 레벨 최적화**를  
핵심 철학으로 개발된 개인 게임 엔진 프로젝트입니다.

STL 컨테이너를 대체하는 커스텀 컨테이너부터 ECS 프레임워크, 메모리 할당자, 런타임 리플렉션 시스템까지   
엔진의 코어 시스템을 직접 설계하고 구현하여 성능과 메모리 레이아웃에 대한 완전한 제어권을 확보했습니다.

---

## 🏗️ 아키텍처 및 핵심 모듈

이 엔진은 `git submodule`을 통해 모듈화된 설계를 따르며, 각 시스템은 독립적으로 재사용 가능하도록 설계되었습니다.

### 1. ECS (Entity Component System)
성능과 확장성을 고려한 고성능 ECS 프레임워크입니다.
- **하이브리드 스토리지** : **Archetype** 패턴의 빠른 순회 속도와 **Sparse Set**의 유연한 엔티티 관리 장점을 결합한 **Node** 기반 스토리지를 구현
- **의존성 관리** : **위상 정렬(Topological Sort)**을 사용하는 `SystemManager`를 통해 시스템 간의 실행 순서를 자동으로 스케줄
- **병렬 처리(예정)** : 멀티스레드 환경에서의 안전한 시스템 실행을 지원하도록 설계

### 2. 메모리 관리 (Memory Management)
범용 힙 할당(Heap Allocation)의 오버헤드와 단편화를 방지하기 위해 특수 목적의 할당자를 사용합니다.
- **커스텀 할당자** : **Pool**, **Array** 할당자를 구현하여 상황에 맞는 메모리 관리를 수행
- **스마트 포인터** : `std::shared_ptr`의 오버헤드를 줄이고 객체 수명 주기를 안전하게 관리하기 위한 **Mark and Sweep** 시스템 구현

### 3. 컨테이너 라이브러리 (Container Library)
게임 개발에 최적화된 STL 호환 컨테이너 라이브러리입니다.
- **No STL in Core** : 성능에 민감한 코어 로직에서 표준 STL을 배제하고, 
	메모리 정렬(Alignment)과 캐시 지역성을 보장하는 커스텀 `Vector`, `HashMap`, `Set`을 사용
- **최적화** : 불필요한 분기 예측 실패와 메모리 재할당을 최소화

### 4. 런타임 리플렉션 (Runtime Reflection)
동적 타입 처리를 위한 C++17 기반 리플렉션 시스템입니다.
- **정적 분석** : SFINAE와 템플릿 메타프로그래밍(TMP)을 활용하여 컴파일 타임에 타입 정보 수집
- **직렬화(예정)** : 리플렉션 정보를 바탕으로 게임 객체의 자동 직렬화/역직렬화(JSON/Binary)를 지원하여 저장 및 로드 기능 제공 예정
- **자동화** : 매크로와 비-타입 템플릿 인자(Non-type template parameter)를 활용하여 타입, 속성 및 함수 정보, 등록 가능

### 5. 그래픽스 (Graphics & OpenGL)
- **API** : OpenGL 4.5 Core Profile 사용
- **Windowing** : **WGL (Win32 API)**을 직접 사용하여 윈도우 생성 및 컨텍스트 제어 과정을 관리
- **RHI 아키텍처(예정)** : 하이 레벨 렌더러와 로우 레벨 API 호출을 분리하는 **RHI (Render Hardware Interface)** 구조

---

## 📂 프로젝트 구조

```text
GameEngine/
├── external/
│   ├── CMake/        # CMake 유틸리티 함수 및 설정
│   ├── Container/    # 커스텀 컨테이너 (Vector, Map, Set 등)
│   ├── ECS/          # 데이터 지향 ECS 프레임워크
│   ├── glad/         # OpenGL 로더 및 WGL 확장
│   ├── glm/          # OpenGL 수학 라이브러리
│   ├── Log/          # 로깅 시스템
│   ├── Memory/       # 커스텀 메모리 할당자
│   └── Reflection/   # 런타임 리플렉션 및 직렬화
├── include			  # 엔진 코어 헤더 파일
├── src/              # 엔진 코어 소스 코드
├── assets/           # 셰이더, 텍스처, 모델 리소스
└── CMakeLists.txt    # 메인 빌드 설정

```

## 🚀 빌드 및 실행 방법

### 요구 사항 (Prerequisites)

* **OS:** Windows 10/11 (64-bit)
* **Compiler:** MSVC (Visual Studio 2019 이상), C++17 지원 필요
* **Tools:** CMake (3.15 이상), Git

### 프로젝트 클론

이 프로젝트는 서브모듈을 포함하고 있으므로 반드시 재귀적으로 클론해야 합니다.

```bash
git clone --recursive [https://github.com/YourUsername/GameEngine.git](https://github.com/YourUsername/GameEngine.git)
cd GameEngine

```

*이미 서브모듈 없이 클론했다면:*

```bash
git submodule update --init --recursive

```

### CMake 빌드

1. 빌드 디렉토리 생성:
```bash
mkdir build
cd build

```


2. 프로젝트 파일 생성:
```bash
cmake ..

```


3. 빌드 실행:
```bash
cmake --build . --config Release

```



---

## 🛠️ 기술 스택 (Tech Stack)

* **Language:** C++17
* **Graphics API:** OpenGL 4.5, WGL (Windows Graphics Library)
* **Math:** GLM (1.0.1)
* **Build System:** CMake

## 📝 라이선스 (License)

이 프로젝트는 MIT 라이선스 하에 배포됩니다. 자세한 내용은 `LICENSE` 파일을 참고하세요.
