# 1
Win API 기본 프레임 워크 구성
-> main.cpp
	-> System Class : Window 생성 및 Main Loop
		-> Input Class : 사용자 입출력
		-> DXEngine Class : Directx 사용

# 2
DirectX 초기화 관련 Class 추가
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D : DirectX 11 초기화

# 3
2차원상의 삼각형 그리기 / Model, Camera, Shader Class 추가
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
			-> DXShader Class : Shader 관련

# 4
2차원상의 삼각형 색깔 바꾸기 - 동적 리소스 사용 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
			-> DXPhysics Class : 물리 연산 관련


# 5
간단한 조명 만들기 - 동적 리소스 사용 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
				-> Light 적용할 수 있게 변경
			-> DXPhysics Class : 물리 연산 관련
			-> DXLight Class : Diffuse Light, Light Position 설정 관련


-> 조명 관련

 Normal Vector : 주어진 Object에서 나오는 방향
 Light Vector : 빛 벡터
 Light Intensity : 채도
 	-> Light Intensity = dot( Normal Vector * ( -1.0f ) , Light Vector )
 	-> 주어진 Object에 빛이 가해지는 정도 ( 0 ~ 1 )

 Diffuse Light : 분산광
 	-> 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
 	-> 각도에 따라 밝기가 다름
 		-> Light Intensity
 	-> Color = material Color * ( Light color * Light Intensity )
 Ambient Light : 주변광
  	-> 수많은 반사를 거쳐서 광원이 불분명한 빛
 	-> 물체를 덮고 있는 빛이며, 일정한 밝기와 색을 표현
 	-> Color = material Color * Ambient Light Color
 Specular Light : 반사광
 	-> 분산광과 달리 한방향으로 완전히 반사되는 빛
 	-> 반사되는 부분은 흰색의 광으로 보임
 Total Light : 실질적인 Object의 빛의 밝기


# 6
3차원 박스 만들기 / MDParticle, DXPhysics 추가 예정
-> main.cpp
	-> System Class
		-> Input Class
		-> DXEngine Class
			-> DXD3D Class : DirectX 초기화 관련
			-> DXCamera Class : Camera 관련
			-> DXModel Class : Model 생성 ( Vertex, Index )
				-> MDParticle Class : 입자 관련 구조체
			-> DXShader Class : Shader 관련
				-> Light 적용할 수 있게 변경
			-> DXPhysics Class : 물리 연산 관련
			-> DXLight Class : Diffuse Light, Light Position 설정 관련

-> Normal Vector
	-> final Normal Vector = World Matrix * Normal Vector
	-> World가 이동 혹은 회전함에 따라 최종 Normal Vector가 달라짐
	-> 이러한 Normal Vector는 Light로 표현되는 Color에도 영향을 끼침

# 7
목록
	-> 프레임워크 재정의
	-> 정반사광 추가
		-> 진짜 간단한 빛 구현
	-> 움직이는 3차원 박스 구현
	-> 간단한 글자 화면에 출력
	-> 사용자의 마우스 입력 관련 구현

구성
-> main.cpp
	-> SYSTEM
		-> DXENGINE
			-> DXD3D
			-> DXCAMERA
			-> DXMODEL
			-> DXLIGHT
			-> DXTEXT
			-> DXTEXTURE
		-> SUB
			-> SUBCPU
			-> SUBFPS
			-> SUBINPUT
			-> SUBLOG
			-> SUBTIMER

내용
-> main.cpp
	-> System
		- Window 초기화
		- MainLoop
		- DX, SUB 관리

		-> DXENGINE
			-> DXD3D
				- DirectX 초기화
				- Device, DeviceContext 관련
				- SwapChain, RenderTarget, DepthStencil, Blending 관련
				- World, Ortho, Projection Matrix & ViewPort 관련

			-> DXCAMERA
				- Camera 초기화
				- Camera Position, Rotation 관련
				- Camera View Matrix 관련

			-> DXMODEL
				- Model 초기화
					- Model Vertex, Index txt 파일 읽기 관련
						- Load Model
					- Image(texture) 관련
						- Load Texture
				- Vertex, Index 관련
					- Update, Init

			-> DXLIGHT
				- Light 초기화
					- Light 방향 및 각 성질들 설정
						- Set ... & Get ...
				-> DXL_SHADER
					- Light Shader 초기화
						- Light VS, PS hlsl 파일 읽기 관련
							- Init ...
					- Layout, SampleState, Buffer 관리
					- Light Render 관리

			-> DXTEXT
				-> DXT_FONT
					- Font 초기화
						- Font DDS(texture), txt 파일 읽기 관련
							- Load DDS & txt
					- 주어진 Text의 Vertex, Index 형성 관련
						- BuildVertexArray
				-> DXT_SHADER
					- Font Shader 초기화
						- Font VS, PS hlsl 파일 읽기 관련
							- Init ...
					- Layout, SampleState, Buffer 관리
					- Font Render 관리

			-> DXTEXTURE
				- Texture 파일 읽기
				- Texture 파일 내용 전해주기

		-> SUB
			-> SUBCPU - CPU 사용량 계산
			-> SUBFPS - FPS 계산
			-> SUBINPUT - User Input 관리
			-> SUBLOG - Log 출력 도우미
			-> SUBTIMER - Time 계산

# 8
--> DXTEXT에서 Sentence 생성 & Update 관련하여 밑의 내용 공부하자
	매개변수(parameter)와 인자(argument)
		-> 함수
			-> 인자(argument)가 매개변수(parameter)에 전달되는 형태로 동작

		ex)
		void func( int a, int b )
		{
			...
		}

		int main()
		{
			func( 1, 2);
			...
		}

		함수가 호출되게 되면,
			int x = 1;
			int y = 2;
		와 같이 인자가 매개변수에 전해짐

	인수 전달
		-> 값에 의한 전달
			-> 함수의 지역변수이나 매개변수로 인자의 값 복사
			-> 함수 호출이 끝나면 인자의 값 변화 X
			-> call by value 라고 부름

		-> 참조에 의한 전달
			-> 매개변수로 전해지는 인자의 메모리를 공유
				-> 매개변수와 인자의 주소 동일
			-> 함수 호출이 끝나면 인자의 값 변화 O
			-> C/C++은 return이 하나만 가능하기에, 이의 방법을 주로 사용
			-> 참조 방식
				-> 일반 & : L-value( 메모리 공간 O )
				-> const & : L-value, R-value( 메모리 공간 X )

		-> 주소에 의한 전달
			-> 값이 주소값으로 받는다고 생각하면 된다
			-> 간접 참조를 통하여 인자의 값 변경 가능
				-> 인자의 주소값을 매개변수가 지니고 있고
				-> 매개변수의 역참조를 통하여 인자로 접근하여 값 변경 가능

# 9
FPS, CPU 화면에 출력

구성
-> main.cpp
	-> SYSTEM
		-> DXENGINE
			-> DXD3D
			-> DXCAMERA
			-> DXMODEL
			-> DXLIGHT
			-> DXTEXT
			-> DXTEXTURE
		-> SUB
			-> SUBCPU
			-> SUBFPS
			-> SUBINPUT
			-> SUBLOG
			-> SUBTIMER

# 10
지면과의 충돌 구현
-> 3차원상의 강체 충돌 구현
	-> CCW(Counter Clockwise) 알고리즘
		-> Object
			- Normal Vector에 따라 Plane의 개수 설정
			- 총 Plane * Plane의 선분 개수
			- ex) 직육면체 2개 충돌 비교
				각각의 Plane의 개수 : 6개
				각 Plane의 선분의 개수 : 4개
				총 계산량 : 6*4*6*4 -> 매우 많아진다

			- 서로 다른 Object끼리 Normal Vector의 내적이 음수인 경우끼리 충돌 가능
				> 음수, 0, 양수 셋 다 계산하게 될 시, 계산량이 너무 많아짐
				> 충돌 시, Normal Vector를 정하는 기준은?

	-> mappedResource를 불러와서, Vertex와 Index에 memcpy를 시킨다.
		-> Output Buffer를 사용하여서 Input Buffer를 통해서 최종 생성된 Output Buffer를 얻는다
		ex memcpy( List, mappedResource.pData , sizeof( Type ) * Count )
		 동적 리소스를 진행했던 형식의 반대라고 생각하면 된다.
		 리소스에 대한 변환은 없다

	-> 강체의 이동
		-> Vertex의 변환
			-> Particle이 아닌 이상, 하나의 Object엔 수많은 Vertex와 Index가 존재
		-> World의 변환
			-> World의 변환을 사용해야
			-> 하나의 Model에 대한 다양한 물체들을 단순히 World를 추가함으로 구현할 수 있음

	-> 추후에 Model List 필요
		-> Model List에서 자신으로부터 Hit Box 범위 안에 있는 Model들만 충돌 테스트할 수 있도록 알고리즘 구현 필요

-> Vertex Shader의 Output Vertex 가져오
	-> D3D11_BIND_FLAG
		-> D3D11_BIND_VERTEX_BUFFER
			IA 단계에서 Binding
		-> D3D11_BIND_INDEX_BUFFER
			IA 단계에서 Binding
		-> D3D11_BIND_CONSTANT_BUFFER
			Shader 단계에서 Binding
		-> D3D11_BIND_SHADER_RESOURCE
			Shader 단계에서 Binding
		-> D3D11_BIND_STREAM_OUTPUT
			Stream Output 단계에서 출력 버퍼 바인딩
		-> D3D11_BIND_RENDER_TARGET
			Output Merge 단계에서 렌더링 대상 바인딩
		-> D3D11_BIND_DEPTH_STENCIL
			Output Merge 단계에서 깊이 스텐실 바인딩
		-> D3D11_BIND_UNORDERED_ACCESS
			순서가 지정되지 않은 액세스 리소스
		-> D3D11_BIND_DECORDER
			디코더 API
		-> D3D11_BIND_VIDEO_ENCODER
			비디오 API



[ 2023-03-18 ]

template< class T >

#include <iostream>

class ResourceManager
{

	template< class T >
	static void CreateMap()
	{
		m_ResourceData[ &typeid( T ) ] = std::map< std::string, IResource* >();
	}

	template< class T >
	static void CreateResource( std::string Name )
	{
		UUID Id;
		IResource* Temp = new T( Name, ID );
		m_ResourceData[ typeid(T).name() ][ ID ] = Temp;
	}

	template< class T >
	static T* GetResource( std::string ID )
	{
		return (T*)m_ResourceData[ typeid(T).name()][ ID ];
	}
	
	template< class T >
	static std::map< std::string, IResource* > > GetMap()
	{
		return m_ResourceData[ typeid(T).name() ];
	}

	std::map< std::string, std::map< std::string, IResource* > > m_ResourceData;	

	std::map< const type_info&, std::map
};

class ResourceGuiLayer
{
	template< class T >
	void RenderTree()
	{
		if ( ImGui::TreeNode( typeid(T).name().c_str() );
		for ( auto itr = ResourceManager::GetMap<T>().begin(); itr != ResourceManager::GetMap<T>().end(); itr++ )
		{
			if ( ImGui::Selectable( itr->GetName().c_str() ) )
			{
				m_PropertyGuiLayer->Render<T>( (T*)itr );
			}
		}
		ImGui::PopNode();
	}
}

class PropertyGuiLayer
{
	void RenderCommon( IResource* Resource )
	{
		ImGui::Text( Resource->GetName().c_str() );
		ImGui::Text( Resource->GetID().c_str() );
		ImGui::Text( Resource->GetType().c_str() );
	}

	void RenderProperty( Scene* Resource )
	{
		...
	}

	void RenderProperty( Component* Resource )
	{
		...
	}
		...

	template< class T >
	void Render( T* Resource )
	{
		RenderCommon();
		RenderProperty( Resource );
	}

	->> SettingGuiLayer, FileSystemGuiLayer처럼 Begin과 End로 구현하기
}

- >> Done


class IResource
{
	std::string m_Name;
	std::string m_UUID;
};

[ 2023-03-21 ]

- Input New Object
GuiLayer ->(Change) IGuiLayer

IBrowserGuiLayer <-(Inheritance) IGuiLayer

MenuBrowserGuiLayer <-(Inheritance) IBrowserGuiLayer
	->> MainGuiLayer

PropertyBrowserGuiLayer <-(Inheritance) IBrowserGuiLayer
	->> PropertyGuiLayer

FileBrowserGuiLayer <-(Inheritance) IBrowserGuiLayer
	->> FileSystemGuiLayer

ProjectManager
	->> Manager Yaml File

- File Browser system
	-> IBrowserGuiLayer
		->> Menu ... ( MenuBrowserGuiLayer )
		->> Property ... ( PropertyBrowserGuiLayer )

		Click selectable for button like "File Load",
		then, Begin FileBrowserGuiLayer

	-> FileBrowserGuiLayer
		Render files and user select path ...
		As result, user click "Done" button,
		then call IBrowserGuiLayer end

	-> MenuBrowserGuiLayer
		if it is called by FileBrowserGuiLayer,
		Call ProjectManager

	-> PropertyBrowserGuiLayer
		if it is called by FileBrowserGuiLayer,
		Call Resource's Compile( FS::path Path ) function


- Change Resource System
	-> ResourceManager
		- Resource must be accessed by ResourceManager,
		because, the other method is existed that access to resource,
		the other method must copy resource's pointer data and
		the other method must overlap ResourceManager

	-> Scene
		- It has map data structure that has typeinfo and UUID vector

	-> Component
		- It has map data structure that has typeinfo and UUId vector


[ 2023-04-01 ]
class SceneManager
{
private :
	SceneManager() {};
	~SceneManager() {};

public :
	void Init() {};
	void Destroy() {};

	static std::vector< Scene >& GetScenes()
	{
		return m_SceneManager.m_SceneData;
	}

	static void SetScene( Scene Other )
	{
		m_SceneManager.m_SceneData.push_back( Other );
	}

private :
	static SceneManager m_SceneManager;
	std::vector< Scene > m_SceneData;
};

class RenderSystem;
class CollisionCheckSystem;

class Scene
{
	public :
		Scene() {};
		~Scene() {};

	public :
		void Init()
		{
			R
		}

		void RegitserEntity( Entity& OtherEntity )
		{

		}

		void RegisterComponent( Component& OtherComponent )
		{

		}

		void RegisterSystem( System& OtherSystem )
		{

		}

		void OnUpdate()
		{

		}
};

class Entity
{
	public :
		Entity() {};
		virtual ~Entity(){};

	public :
		virtual void Update() = 0;
};

struct Component
{

};

class System
{
	public :
		System() {};
		virtual ~System() {};

	public :
		virtual void Init() = 0;
		virtual void Update() = 0;
};

struct Transformation
{
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;
};

struct FilePath
{
	FS::path Path;
};

struct Mesh
{
	VertexBuffer;
	IndexBuffer;
};

struct Renderable
{
	Trnasformation* m_Transform;
	Mesh* m_Mesh;
};

[ 2023-04-07 ]

// Component들
struct IDComponent
{
	MyUUID ID;
};

struct TagComponent
{
	std::string Name;
};

struct TransformComponent
{

};

// Entity 객체
class Entity
{
	public :

	public :
		void AddComponent();

};

// Node들
struct RenderableNode
{

};

struct CameraNode
{

};

struct LightNode
{

};

// System들
class RenderingSystem
{

};

[ 2023-04-09 ]

Entity
	-> Component에 대한 주소들을 지니고 있음
	-> 단순히, Component들을 등록 및 해제할 수 있음

Component
	-> 특성들을 지니고 있음

Node
	-> Archtype이라고 생각하면 될 듯
	-> System으로 넘겨야 되는, 각 Entity에 따른 Component들의 묶음이라고 생각
	-> Component에 대한 주소들을 지니고 있음
	-> Entity에 속한 작은 Entity라고 생각하면 될 듯

Scene ( Entity Manager )
	-> Entity들을 관리
	-> Entity 생성 및 삭제

Component Manager
	-> Component들의 실제 인스턴스들을 저장
	-> Component들을 생성 및 삭제

Node Manager
	-> Node들의 실제 인스턴스들을 저장
	-> Node들에게 해당 Entity들을 넘겨주는 역할( Scene에서 받아옴 )

의문점
	1. Entity들은 어떻게 해서 자신이 가져야 할 Node들의 정보를 알 수 있지?
		1.1 사용자가 설정할 수 있도록 해야한다.
		1.2 해당 Entity가 어떤 Node들을 가지고 System에 연결할 지 어떻게 할 것인가?
		1.3 Proj 파일 안에서 해당 Entity들이 어떤 Node들에 연결이 될 지 나타낼 수 있다.
	2. 해당 Node들과 System들은 보통은 1대1로 매칭시키자

	3. Blue Print처럼 Node들을 연결시킬 수 있는 Visual Graph를 구현하는 것이 좋을까?
		3.1 연결에 대한 정보는 Proj 파일에 들어가 있도록 구현을 해보자
		3.2 만약에 연결을 직접 해야한다면 어떻게 해야할까?
		3.3 Visual Graph가 없을 때는 어떤식으로 구현할지 생각을 해보자.

[ 2023-04-10 ]
1. 차세대 Unit의 기반 기술
2. Data Oriented Design( DOD ) 으로의 전환
3. ECS, Job System. Burst Compiler 세가지 요소의 합
4. 복잡한 코드없이 간단하게 Multi-Threading이 가능
5. 빌드 옵션 하나로 퍼모먼스 향상

# Entity, Component, System
	-> DOD를 위해 제공되는 아키텍쳐
	-> Entity는 데이터를 가리키는 인덱스
	-> Component는 읽고 쓰기 위한 데이터
	-> System에서 Entity와 Component를 가지고 로직을 구현
	-> Entity와 Component만 있다면 아무 일도 일어나지 않음

# Job System
	-> Multi-Threading을 가능하게 하는 시스템
	-> Race Condition 걱정없이 Threading 구현이 가능
	-> 코드상의 이름은 Job Component System
	-> Job들을 생성하고 의존성에 대한 Handle을 반환
	-> 작업 방식에 따라 여러 형태의 Job을 제공

#1. Entity
-> ID들을 지니고 있다.
-> Component들에 대한 포인터를 지니고 있다.

#2. Component
-> 실제의 Attribute들을 지니고 있다.

#3. Node
-> 해당된 Entity의 ID를 등록한다
-> Component들의 포인터를 지니고 있다.
	-> Node들을 어떻게 해서 Caching Hit를 높일지는 나중에 생각을 해보자.

#4. System
-> 필요로 하는 Node들을 가지고 Update를 진행한다.

struct Component
{
// attribute for entities
};

class Entity
{
// Register Component;
// Make Entity ID;
};

struct Node
{
// This is component collection
};

[ 2023-04-11 ]

#1. Map 구조
	-> Component Manager
		Key : Component Type 정보
		Value : std::any
			-> std::vector< Type >
			-> std::any_cast 를 사용해서 구현을 해보자.

		or C++17 이하와의 호환성을 위해서
		Value : void*
			-> std::vector< Type >

	-> Entity
		Key : Component Type 정보
		Value : void*

	-> Scene
		Key : MyUUID
		Value : Entity

	-> Scene Manager
		Key : MyUUID
		Value : Scene

	-> Node Manager
		Key : Node Type 정보
		Value : std::any
			-> std::vector< Type >

		or C++17 이하와의 호환성을 위해서
		Value : void*
			-> std::vector< Type >

		하지만 이미 typeinfo를 사용해서 C++17 이상만 사용가능한 것 아닌가?

	-> void* reinterpret_cast vs std::any
		-> reinterpret_cast 포인터 간의 형변환
			-> 잘못된 변화일 때의 경우, 막아주는 것이 없다.

		-> std::any
			-> 잘못된 변환일 때, 막아줄 수 있게 해준다.

	-> map 자체는 그냥 48 바이트의 크기를 지닌다.
		실제 map 객체 전체의 데이터의 크기는
		Key + Value * map pair 개수 + map instance 사이즈이다.

		pair 데이터의 경우에는 Heap에 저장되고( 동적 )
		map 객체 자체이 경우에는 Stack에 저장( 정적 )
		익 대문에, map 객체 자체에 대한 사이즈는 48로 일정할 수 밖에 없다.

	-> Layer들의 경우에는 Component에 따라서 Rendering 해주는 Property Layer가 존재하고
	-> Scene들을 다루는 Scene Layer
	-> Log들만을 주로 다루는 Log Layer
	-> Main Menu Layer가 존재하게 된다.


Entity
	-> ID
	-> Component's Pointer

Node
	-> Id
	-> Component's Pointer

Componen Manager
	-> Type Component Instance

Node Manager
	-> Type Node Instance

Scene
	-> ID
	-> Entity Instance


[ 2023-04-12 ]

# Entity
	-> ID
	-> Components' Pointer

# Node
	-> ID
	-> Components' Pointer

# Component Manager
	-> Each Type of Component Instance
	-> std::any or void* : std::vector

# Node Manager
	-> Each Type of Node Instance
	-> std::any or void* : std::vector

# Scene
	-> ID
	-> Entities' Instance

# Scene
	-> Scenes' Instance

# System
	-> The Function uses some type of Node
	-> The System Needs very complex, and 
	each system cannot contact each other,
	just connect through node
	
# System Manager
	-> Systems' Instance
	-> Register System
	-> Systems must be inheritance ISystem that has init,set and frame function's interface
	-> System Manager set sequence of System on frame
	
	# ISystem
		-> That is the interface of system
		-> This Object has some pointer of prev, next ISystem.
		-> Cause, System must have seqeunce during frame animation
		-> For example, Collision Animation has first collision System,
		Second CalculatePhysicsSystem, lastly RenderSystem.
		-> So, if this sequence is linked list, the User using this engine,
		more easily edit sequence, some system input or delete ... 
		and, user can see thie sequence on game engine ui.
		So, the User drag some system for inputting this sequence, 
		just, connect prev system and next system.

[ 2023-05-10 ]
## Category
	0. Global Variable ( static variable )
		-> This Object is static variable
		-> This Object is global variable

	1. Handler	
		-> Manage without Entity, Component, Node, System, Scene
		-> Manage Asset, Files ... etc

	2. Manager
		-> Manage Entity, Component, Node, System, Scene
	
	3. ECS( Entity Component System )
		-> Entity
			-> Connection of Components
			-> This object must have unique number that is called as UUID
		-> Component
			-> This object is actial data
		-> System
			-> This object is acting
			-> This object run using Node or Component

		-> Node
			-> This object is unrealitic data's or data's connection by entity
		-> Scene	
			-> This object is connection of Entities.

## RelationShip
	0. Global Variable ( static variable )
		0.1 Core
			0.1.1 Core
			0.1.2 Log
			0.1.3 Timer
		0.2 API
			0.2.1 WinAPI
			0.2.2 DirectXAPI
			0.2.3 ImGuiAPI
		0.3 Handler
		0.4 Manager

	1. Handler
		1.1 AssetHandler
		1.2 FileHandler

	2. Manager
		2.1 SceneManager
		2.2 Scene( EntityManager )
		2.3 ComponentManager
		2.4 NodeManager
		2.5 SystemManager

	3. ECS ( Entity Component System )
		3.1 Entity
		3.2 Component
		3.3 System
		3.4 Node
		3.5 Scene ( Entity Manager )

[ 2023-05-11 ]
## Scene's work
	1. Make Entity
	2. Add Entity
	3. Remove Entity

## ComponentManager's work
	1. Make Component
	2. Add Component
	3. Remove Component

## NodeManager's work
	1. Make Node through Entity's ID
	2. Add Node through Entity's ID
	3. Remove Node through Entity's ID

## Entity's work
	1. Add Component -> Through ComponentManager
	2. Make Component -> Through ComponentManager
	3. Give Entity's ID
	4. Give Entity's Name

## Property UI's work
	1. Render Entity's Component -> Through NodeManager;
	2. Edit Entity's Component -> Through NodeManager;
	3. Render Array of Entity's Node -> Through NodeManager;
	4. Edit Entity's Node -> Through NodeManager;

	5. Render Scene's System -> Through SystemManager;
	6. Edit Scene's System Sequence -> Through SystemManager;

## Main UI's work
	1. Open Project File -> Through FileHandler + YAML
	2. Save Project File -> Through FileHandler + YAML
	3. Make Scene, Component, Node -> Through Scene, Component, Node Manager
	4. Register System -> Through SystemManager

## Scene Array UI's work
	1. Select Scene -> Through SceneManager
	2. Make Scene -> Through SceneManager
	3. Remove Scene -> Through SceneManager
	4. Select Entity -> Through Scene
		4.1. Send Entity's ID to Property UI
	5. Make Entity -> Through Scene
	6. Remove Entity -> Through Scene

## Log UI's work
	1. Print Log -> Through Log
	2. Save Log File -> Through AssetHandler

[ 2023 - 05 - 13 ]
## Critical Problem
	1. The entity, node have some components that they need
	2. The Component Manager has each type of Component's Vector
	3. These Vector reallocate when user input Component
	4. Then, entity and node would miss pointer of their Component

## Solution
	1. Using Index of Component Vector, instead Pointer
		1.1 Entity and Node just have index for each Component
		1.2 Through ComponentManager, Node gives information of index's Component Vector to each System.
		1.3 Node get information of index's Component through entity's Instance.

[ 2023 - 05 -18 ]
## Placement New 
	1. Using Interface, user easily make Instance.
	2. The origin method is just making instance using std::any with vector.
		So, the program must change type of std::any using std::any_cast.
	3. This is very useful in advanced game engine's memory manager.

## Memory Pool
	1. Each type of object vector or list just have memory pool address or register memory pool.
	2. Then, user can access each type of memory pool, get continuously data in memory.
	3. But, if the vector's data count of allocating data is over then  memory pool size.
	4. the program must allocate new memory pool and connect the origin pool.
	
## Using RTTI with typeid
	1. The inheritance relationship using interface and instance, can access through interface's pointer to instance
	2. The Manager like System, Node, Component have the method function
		like, Create, Get, Delete.
	3. The original method functions only access through template typename T each type's vector.
	4. But, this needs the program is created by hard coding for suitable each type.
	5. So, this needs the user input new type of system or node or component, 
		change the original code.
	6. The changine the original core code is so difficult.
	7. I want to just use the Outter code in my game engine.
	8. So, the Managers of System, Component, Node have two type of eacy method functions for typeid and template
	9. But, template method function is constructed by typeid method function.

## ECS that was made by other user
	## WebSite : https://github.com/talhacali/ECS
		1. Using Memory Manager, this object allocate memory.
		2. This object has memory's pointer by each type.
		3. The ECS manager create their object through memory manager.
		4. The ECS manager just have pointer of each object.
			So, just use vector or map.