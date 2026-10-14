// 셰이더 공용 정의 (GLSLParser가 #include를 텍스트로 펼침 : #version 다음 줄에서 include할 것)
// 구조체, binding 번호, 블록 선언을 이 파일 한 곳에서만 정의함. MaterialGraph가 생기면 이 파일의 머티리얼 부분이 생성 대상이 됨

// ---------------------------------------------------------------------------
// 인스턴스 번호
//   RHI_InstanceIndex : base instance가 포함된 번호 (firstInstance + 0 기반 번호)
//   RHI_InstanceID    : 0 기반 번호
// VULKAN은 Vulkan 타깃으로 컴파일할 때 컴파일러(glslang / shaderc)가 정의함. GL 경로에는 정의되지 않음
// 요구 사항 : Vulkan 1.1 (shaderDrawParameters), OpenGL 4.6
// ---------------------------------------------------------------------------
#ifdef VULKAN
	#define RHI_InstanceIndex	gl_InstanceIndex
	#define RHI_InstanceID		(gl_InstanceIndex - gl_BaseInstance)
#else
	#define RHI_InstanceIndex	(gl_InstanceID + gl_BaseInstance)
	#define RHI_InstanceID		gl_InstanceID
#endif

// ---------------------------------------------------------------------------
// binding 번호 : 같은 이름은 모든 셰이더에서 같은 번호. 한 파이프라인 안에서 번호가 겹치면 안 됨
// (Vulkan은 UBO / SSBO / 텍스처가 번호 공간을 공유함). PSO 생성 때 엔진이 불일치를 검증함
// ---------------------------------------------------------------------------
#define BINDING_CAMERA			0
#define BINDING_TRANSFORM		1
#define BINDING_MATERIAL		2
#define BINDING_LIGHT			3
#define BINDING_RAW_TRANSFORM	4
#define BINDING_DIFFUSE			5
#define BINDING_G_DEPTH			6
#define BINDING_G_NORMAL		7
#define BINDING_G_ALBEDO		8
#define BINDING_G_PHONG			9
#define BINDING_G_SHADOW		10
#define BINDING_G_PARAM			11
#define BINDING_LIGHT_PASS		12

// 입출력 location : 정점 입력은 GetVertexLocation (position 0, normal 1, texcoord 6), varying은 normal 0 / texcoord 1

// ---------------------------------------------------------------------------
// 구조체 (CPU 쪽 구조체와 std140 / std430 배치가 같아야 함)
// ---------------------------------------------------------------------------
struct CameraData
{
	mat4 view;
	mat4 proj;
	mat4 invView;
	mat4 invProj;
	vec3 position;
	vec3 direction;
};

struct MaterialPhong
{
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
	float opacity;
};

struct MaterialPBR
{
	vec3 baseColor;
	float roughness;
	float metallic;
	float ambientOcclusion;
	float opacity;
};

struct DirectionalLight
{
	vec3 color;
	vec3 position;
	float intensity;
	vec3 direction;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	float intensity;
	float range;
};

struct SpotLight
{
	vec3 color;
	vec3 position;
	float intensity;
	vec3 direction;
	float range;
	float innerAngle;
	float outerAngle;
};

// ---------------------------------------------------------------------------
// 블록 선언 : 쓰는 셰이더만 선언함 (선언하지 않으면 PSO에 바인딩이 생기지 않음)
// 블록 이름(uCamera, uLight ...)이 리플렉션 키이고, 인스턴스 이름(camera, light ...)으로 접근함
// ---------------------------------------------------------------------------
#define DECLARE_CAMERA				layout(std140, binding = BINDING_CAMERA) uniform uCamera { CameraData camera; };
#define DECLARE_TRANSFORM			layout(std430, binding = BINDING_TRANSFORM) readonly buffer uTransform { mat4 datas[]; } transform;
#define DECLARE_MATERIAL_PHONG		layout(std430, binding = BINDING_MATERIAL) readonly buffer uMaterial { MaterialPhong material; };
#define DECLARE_MATERIAL_PBR		layout(std430, binding = BINDING_MATERIAL) readonly buffer uMaterial { MaterialPBR material; };
#define DECLARE_LIGHT_DIRECTIONAL	layout(std430, binding = BINDING_LIGHT) readonly buffer uLight { DirectionalLight light; };
#define DECLARE_LIGHT_POINT			layout(std430, binding = BINDING_LIGHT) readonly buffer uLight { PointLight light; };
#define DECLARE_LIGHT_SPOT			layout(std430, binding = BINDING_LIGHT) readonly buffer uLight { SpotLight light; };

// 라이트 패스 상수 : 낱개 uniform 대신 이름 있는 UBO 블록 하나로 받음 (엔진 내부 링 버퍼가 draw마다 슬라이스를 바인딩)
// 블록 전체를 선언해야 offset이 맞으므로 일부만 써도 매크로로 통째로 선언할 것
#define DECLARE_LIGHT_PASS			layout(std140, binding = BINDING_LIGHT_PASS) uniform uLightPass { mat4 uLightView; mat4 uLightProj; };
