# Custom Game Engine

## Project Overview

This project is a **C++17 Data-Oriented game engine built from scratch**, referencing the architecture of **Unreal Engine**.

The goal is to design and implement the core structures found in commercial engines: **Runtime Reflection**, **Garbage Collection (Mark & Sweep)**, **ECS (Entity Component System)**, and the key rendering pipeline concepts of **MeshBatch**, **PrimitiveProxy**, **RenderGraph**, and **RHI (Render Hardware Interface)**.

Every core layer of the engine is directly controlled — from STL-free custom containers and purpose-built memory allocators to a multi-threaded worker-based pipeline — pursuing a complete understanding of both performance and architecture.

## 📸 Current Progress & Demo

**Milestone: Core Framework & Basic Rendering Completed** Currently, the core backbone of the engine, asset parsers, and the foundational RHI rendering pipeline have been successfully implemented.

*[26-04-17] Demonstration of MeshBatch Instancing with Normal Mapping*
![MeshBatch Instancing Demo](asset/screenshot/base-dragon-viewer.gif)

---

## Core Design Philosophy

* **Unreal Engine Reference & Reinterpretation** : Implements core rendering concepts from Unreal Engine such as `PrimitiveProxy`, `RenderScene`, `RHICommandList`, and `RenderGraph`
* **Data-Oriented Design (DOD)** : Component/Node layout designed with cache locality and memory contiguity in mind
* **No STL in Hot Path** : Custom `Vector`, `HashMap`, and `HashSet` replace STL in performance-critical paths
* **Multi-threaded Worker Pipeline** : World / Renderer / RHI / Asset each run on independent threads, synchronized via `FrameGate`
* **Runtime Reflection** : `GENERATE`, `PROPERTY`, `METHOD` macros register type information at compile time for runtime access

---

## External Modules (Git Submodules)

| Module | Role |
|--------|------|
| `ECS` | ECS framework based on Entity, Component, Node, System, Graph |
| `Memory` | Pool/Array allocators, RefPtr/ObjectPtr/RootPtr smart pointers |
| `Reflection` | C++17 runtime reflection (TypeInfo, PropertyInfo, MethodInfo) |
| `Container` | STL-free Vector, HashMap, HashSet, StaticArray |
| `Log` | Logging system |
| `glm` | Math library (GLM 1.0.1) |
| `glad` | OpenGL 4.5 loader + WGL (Win32) |
| `imgui` | Debug GUI |
| `yaml-cpp` | Configuration file parsing |
| `stb` | Image loader (stb_image) |

---

## Project Structure

```text
Game-Engine/
├── Engine/
│   ├── Public/                  # Public headers
│   │   ├── Framework/           # Engine, Task, TaskWorker, Window, Input
│   │   ├── World/               # World, Entity, Component, Node, System, Commander
│   │   ├── Renderer/            # Renderer, RenderGraph, PipeLine, RenderTypes
│   │   ├── RHI/                 # RHISystem, RHIResources, RHICommandList, RHICommands
│   │   └── Asset/               # AssetSystem, AssetTypes, AssetFactory, Parsers
│   └── Private/                 # Internal implementation
│       ├── Framework/           # Worker, FrameGate
│       ├── World/               # WorldWorker, WorldContext
│       ├── Renderer/            # RenderWorker, RenderScene, RenderCommandList, MeshBatch, Proxy
│       ├── RHI/                 # RHIWorker, RHIFrameExecutor, RHITaskExecutor, OpenGL/
│       └── Asset/               # AssetWorker
├── external/                    # Git Submodules
├── asset/                       # Shaders, textures, model assets
└── CMakeLists.txt
```

---

## Architecture Diagrams

### 0-A. External Module ↔ Game Engine — Component Dependency

Dependency relationships between external submodules and internal engine modules. `Memory`, `Container`, and `Log` are shared across all layers. `ECS` serves as the foundation framework for the World layer.

```mermaid
flowchart TD
    subgraph External["External Modules (Git Submodules)"]
        ECS[ECS]
        Memory[Memory]
        Reflection[Reflection]
        Container[Container]
        Log[Log]
        glm[glm]
        glad[glad]
        imgui[imgui]
        yaml-cpp[yaml-cpp]
        stb[stb]
    end

    subgraph Engine["Game Engine"]
        Framework[Framework]
        World[World]
        Renderer[Renderer]
        RHI[RHI]
        Asset[Asset]
    end

    ECS -->|Entity / Component<br>Node / System / Graph| World
    Reflection -->|GENERATE / PROPERTY / METHOD<br>Runtime type registration| World
    glm -->|fvec3 / fmat4 math operations| World
    glm -->|Matrix / vector operations| Renderer
    glm -->|RHIDescriptions math types| RHI
    glad -->|OpenGL 4.5 Core Profile<br>API loading WGL| RHI
    imgui -->|Debug GUI rendering| Framework
    yaml-cpp -->|.yaml config file parsing| Asset
    stb -->|stb_image<br>Image byte decoding| Asset

    Memory -.->|RefPtr / ObjectPtr / RootPtr<br>Pool & Array allocators| Framework
    Memory -.-> World
    Memory -.-> Renderer
    Memory -.-> RHI
    Memory -.-> Asset

    Container -.->|DynamicArray / HashMap<br>HashSet / StaticArray| Framework
    Container -.-> World
    Container -.-> Renderer
    Container -.-> RHI
    Container -.-> Asset

    Log -.->|LOGINFO / LOGERROR macros| Framework
    Log -.-> World
    Log -.-> Renderer
    Log -.-> RHI
    Log -.-> Asset
```

---

### 0-B. Game Engine Internal Modules — Component Dependency

Dependency directions and responsibilities of the five internal engine modules. Dependencies always flow **toward lower layers only**.

```mermaid
flowchart TD
    FW["Framework<br>─────────────────<br>Engine · Window<br>Worker · FrameGate<br>Input · Task · Application"]
    WO["World<br>─────────────────<br>ECS wrapper layer<br>Entity · Component · Node<br>System · Commander<br>Scene · WorldContext"]
    RD["Renderer<br>─────────────────<br>PrimitiveProxy · LightProxy<br>RenderScene · MeshBatch<br>RenderGraph · PipeLine<br>GlobalResource"]
    RI["RHI<br>─────────────────<br>RHISystem (GLSystem)<br>RHIResources · RHICommandList<br>RHIFrameExecutor · RHITaskExecutor<br>RHI Command Objects (45+)"]
    AS["Asset<br>─────────────────<br>AssetSystem · AssetFactory<br>OBJ / MTL / PNG / GLSL Parsers<br>Async 3-stage pipeline"]

    WO -->|InputStorage read input<br>FrameGate frame sync participation| FW
    RD -->|RenderCommandList consume<br>RenderScene <- Commander Enqueue| WO
    RD -->|RHICommandList generate GPU commands<br>RHIFrameExecutor.Acquire / Submit| RI
    AS -->|RHITaskExecutor<br>submit GPU upload commands| RI
    FW -->|RHIExecutor lifecycle management<br>Init / Shutdown| RI
    FW -->|WorldContext lifecycle management| WO
    FW -->|Renderer lifecycle management| RD
    FW -->|AssetWorker & TaskWorker management| AS

    FW_Note["Note: Owns and controls all Worker threads.<br>FrameGate synchronizes the<br>World -> Renderer -> RHI frame flow."] -.- FW
    RI_Note["Note: Actual GPU API calls happen only in RHIWorker.<br>RHISystem is a platform abstraction interface<br>(swappable: OpenGL / DirectX / Vulkan)."] -.- RI
```

---

### 1. Engine — Full System Composition

The `Engine` class owns and initializes all workers (threads) and subsystems. `FrameGate` synchronizes the frame flow in World → Renderer → RHI order.

```mermaid
classDiagram
    class Engine {
        - RefPtr~Window~ m_window
        - RefPtr~InputHandler~ m_inputHandler
        - RefPtr~InputStorage~ m_inputStorage
        - RefPtr~FrameGate~ m_updateGate
        - RefPtr~FrameGate~ m_renderGate
        - RefPtr~WorldContext~ m_worldContext
        - RefPtr~Renderer~ m_renderer
        - RefPtr~RHISystem~ m_rhiSystem
        - RefPtr~RHIExecutor~ m_rhiFrameExecutor
        - RefPtr~RHIExecutor~ m_rhiTaskExecutor
        - RefPtr~Worker~ m_worldWorker
        - RefPtr~Worker~ m_renderWorker
        - RefPtr~Worker~ m_rhiWorker
        - RefPtr~Worker~ m_assetWorker
        + Init(windowDesc, renderDesc) bool
        + Shutdown() void
        + Run() void
        + GetInputStorage() RefPtr~InputStorage~
        + GetWorldContext() RefPtr~WorldContext~
        + GetRenderer() RefPtr~Renderer~
        - InitWindow() bool
        - InitRender() bool
        - InitRHI() bool
        - InitWorld() bool
        - InitAsset() bool
        - InitWorker() bool
        - UpdateInput() void
    }

    class Worker {
        <<abstract>>
        - atomic~bool~ m_isRunning
        - thread m_thread
        + Start() void
        + Stop() void
        # onStart() bool
        # onUpdate() void
        # onDestroy() void
        # onNotify() void
        - Run() void
    }

    class TaskWorker {
        - atomic~bool~ m_isWaited
        - condition_variable m_cv
        - mutex m_mutex
        - RefPtr~DefaultTask~ m_task
        + Wait() void
        + Notify() void
        + Set(task) void
        + IsWaited() bool
    }

    class FrameHandler {
        <<abstract>>
        + NotifyAll() void
    }

    class FrameProducer {
        <<abstract>>
        + Submit() void
    }

    class FrameConsumer {
        <<abstract>>
        + Acquire() void
    }

    class FrameGate {
        - condition_variable m_cvProducer
        - condition_variable m_cvConsumer
        - mutex m_mutex
        - size_t m_frameLimit
        - atomic~int32_t~ m_frameDiff
        - atomic~bool~ m_notified
        + Submit() void
        + Acquire() void
        + NotifyAll() void
    }

    class WorldWorker {
        - ECS_TimeStep m_timeStep
        - RefPtr~InputStorage~ m_refInputStorage
        - RefPtr~WorldContext~ m_refWorldContext
        - RefPtr~FrameProducer~ m_refProducer
        - DynamicArray~RenderView~ m_renderViews
    }

    class RenderWorker {
        - RefPtr~Renderer~ m_refRenderer
        - RefPtr~RHIExecutor~ m_refExecutor
        - RefPtr~FrameConsumer~ m_refConsumer
        - RefPtr~FrameProducer~ m_refProducer
    }

    class RHIWorker {
        - RefPtr~RHISystem~ m_refSystem
        - RefPtr~RHIExecutor~ m_refFrameExecutor
        - RefPtr~RHIExecutor~ m_refTaskExecutor
        - RefPtr~FrameConsumer~ m_refConsumer
    }

    class AssetWorker {
        - DynamicArray~RefPtr_TaskWorker~ m_threads
        - RefPtr~RHIExecutor~ m_refTaskExecutor
        + SetTaskThread(count) void
        + SetTaskExecutor(executor) void
    }

    FrameHandler <|-- FrameProducer
    FrameHandler <|-- FrameConsumer
    FrameProducer <|-- FrameGate
    FrameConsumer <|-- FrameGate

    Worker <|-- WorldWorker
    Worker <|-- RenderWorker
    Worker <|-- RHIWorker
    Worker <|-- AssetWorker
    Worker <|-- TaskWorker

    Engine *-- FrameGate : m_updateGate / m_renderGate
    Engine *-- WorldWorker
    Engine *-- RenderWorker
    Engine *-- RHIWorker
    Engine *-- AssetWorker

    AssetWorker *-- TaskWorker
```

---

### 2. World — ECS Game Logic Layer

References Unreal Engine's `UWorld`, `AActor`, `UActorComponent` structure. `Entity` acts as the `Actor`, `Component` holds data, and `Node` handles component composition. `Commander` is the one-way data bridge from World → Renderer.

```mermaid
classDiagram
    class WorldContext {
        + RootPtr~World~ world
        + RefPtr~ViewController~ views
        + RefPtr~PlayerController~ players
        + RefPtr~Commander~ commander
        - RefPtr~WorldCommandList~ m_refCommandList
        + Init() bool
        + Clear() void
        + Prepare() void
        + Update(timeStep) void
    }

    class World {
        <<ECS::Object>>
        + Scene scene
        - ObjectStorage~Entity~ m_entityStorage
        - Registry~BaseComponent~ m_componentContainer
        - Registry~BaseNode~ m_nodeContainer
        - Graph~BaseSystem~ m_systemRegistry
        + Init(refCommander) bool
        + Update(timeStep) void
        + CreateEntity() ObjectPtr~Entity~
        + CreateComponent~T~(...) ObjectPtr~T~
        + CreateNode~T~(...) ObjectPtr~T~
        + CreateSystem~T~(...) RefPtr~T~
        + GetEntity(uuid) ObjectPtr~Entity~
        + RemoveEntity(uuid) void
    }

    class Entity {
        <<ECS::Entity>>
        - World* m_owner
        + Clear() void
        + AddComponent~T~(...) bool
        + AddNode~T~(...) bool
        + GetComponent~T~() ObjectPtr~T~
        + GetNode~T~() ObjectPtr~T~
        + HasAllComponents~TList~() bool
    }

    class Scene {
        - RefPtr~Commander~ m_refCommander
        - HashMap~UUID_ScenePair~ m_sceneDatas
        + Attach(node) void
        + Detach(node) void
        + Detach(entityId) void
        + Update(entityId) void
    }

    class Commander {
        - RefPtr~RenderCommandList~ m_refCmdList
        + SetView(renderView) void
        + AddPrimitive(meshNode) void
        + RemovePrimitive(meshNode) void
        + AddLight(lightNode) void
        + RemoveLight(lightNode) void
        + Update(sceneComponent) void
        + Remove(entityId) void
        + RemoveAll() void
    }

    class BaseComponent {
        <<ECS::Component>>
    }

    class SceneComponent {
        - fvec3 m_position
        - fvec3 m_rotation
        - fvec3 m_scale
        - Scene* m_scene
        + UpdatePosition(pos) void
        + UpdateRotation(rot) void
        + UpdateScale(scale) void
        + OnAttached() void
        + OnDetached() void
    }

    class CameraComponent {
        + float fov
        + float nearPlane
        + float farPlane
        + float width
        + float height
        + bool perspective
    }

    class MeshComponent {
        + RefPtr~MeshAsset~ meshAsset
    }

    class MaterialComponent {
        + RefPtr~MaterialAsset~ materialAsset
    }

    class LightComponent {
        + fvec3 direction
    }

    class ColorComponent {
        + float red
        + float green
        + float blue
        + float alpha
    }

    class BaseNode {
        <<ECS::Node>>
    }

    class MeshNode {
        + ObjectPtr~SceneComponent~ transform
        + ObjectPtr~MeshComponent~ mesh
        + ObjectPtr~MaterialComponent~ overrideMaterial
    }

    class CameraNode {
        + ObjectPtr~SceneComponent~ transform
        + ObjectPtr~CameraComponent~ camera
    }

    class LightNode {
        + ObjectPtr~SceneComponent~ transform
        + ObjectPtr~LightComponent~ light
    }

    class BaseSystem {
        <<ECS::System>>
    }

    class TypedSystem~T~ {
        # UpdateInternal(timeStep, container) void
    }

    class InputSystem~T~ {
        # RefPtr~InputStorage~ inputStorage
    }

    WorldContext *-- World
    WorldContext *-- Commander

    World *-- Scene
    World o-- Entity

    Entity ..> World : uses

    BaseComponent <|-- SceneComponent
    BaseComponent <|-- CameraComponent
    BaseComponent <|-- MeshComponent
    BaseComponent <|-- MaterialComponent
    BaseComponent <|-- LightComponent
    BaseComponent <|-- ColorComponent

    BaseNode <|-- MeshNode
    BaseNode <|-- CameraNode
    BaseNode <|-- LightNode

    MeshNode o-- SceneComponent
    MeshNode o-- MeshComponent
    MeshNode o-- MaterialComponent

    CameraNode o-- SceneComponent
    CameraNode o-- CameraComponent

    LightNode o-- SceneComponent
    LightNode o-- LightComponent

    BaseSystem <|-- TypedSystem
    TypedSystem <|-- InputSystem

    Scene ..> Commander : uses
    Commander ..> RenderCommandList : enqueues
```

---

### 3. Renderer — Rendering Abstraction Layer

References Unreal Engine's `FScene`, `FPrimitiveSceneProxy`, `FMeshBatch`, and `FRenderingCompositePassContext`. World objects are never passed directly to the renderer — they are converted into **Proxy** objects. `RenderScene` reorganizes these Proxies into `MeshBatch` instances to implement instancing.

```mermaid
classDiagram
    class Renderer {
        - RefPtr~RenderScene~ m_refScene
        - RefPtr~RenderGraph~ m_refGraph
        - RefPtr~RenderCommandList~ m_refCommandList
        - DynamicArray~RenderView~ m_renderViews
        + Init() bool
        + Execute(cmdList) void
        + SetView(view) void
        + PreDraw(cmdList) void
        + Draw(cmdList) void
        + PostDraw(cmdList) void
        + GetScene() RefPtr~RenderScene~
        + GetGraph() RefPtr~RenderGraph~
    }

    class RenderScene {
        - HashMap~UUID_RefPtr_PrimitiveProxy~ m_primitives
        - HashMap~UUID_RefPtr_LightProxy~ m_lights
        - HashMap~MeshBatchKey_RefPtr_MeshBatch~ m_meshBatches
        - DynamicArray~RefPtr_PrimitiveProxy~ m_pendingPrimitives
        + Flush(cmdList) void
        + UpdateProxy(updateInfo, cmdList) void
        + AddPrimitive(primitive) void
        + RemovePrimitive(id, cmdList) void
        + AddLight(light) void
        + RemoveLight(id) void
        + AddBatch(primitive, cmdList) void
        + UpdateBatch(primitive, cmdList) void
        + RemoveBatch(primitive, cmdList) void
        + GetMeshBatch(key) RefPtr~MeshBatch~
    }

    class RenderGraph {
        <<ECS::Graph~PipeLine~>>
        - HashSet~RefPtr_PipeLine~ m_addable
        - HashSet~RefPtr_PipeLine~ m_removable
        - RefPtr~GlobalResource~ m_globalResource
        - DynamicArray~RefPtr_MeshDrawCommand~ m_drawCommands
        + Init(cmdList) bool
        + Execute(cmdList, renderScene, renderView) void
        + Flush(cmdList) void
        + Add(pipeline) void
        + Remove(pipeline) void
    }

    class GlobalResource {
        + CameraData cameraData
        + BufferResource bufferResource
        + TextureResource textureResource
        + ScreenQuadResource screenQuadResource
        + Init(cmdList) bool
        + UpdateCamera(renderView, cmdList) void
        + GetCameraBuffer() RefPtr~RHIBuffer~
        + GetScreenQuad() RefPtr~RHIVertexLayout~
    }

    class PipeLine {
        <<ECS::Object, RenderResource>>
        - RefPtr~RHIPipeLine~ m_pipeLine
        - bool m_prepared
        + Execute(commands, globalResource, cmdList) void
        + Init() void
        + Prepare() void
        + Draw(commands, globalResource, cmdList) void
        + GetShaderState() eResourceState
    }

    class SimpleColor {
        + Init() void
        + Prepare() void
        + Draw(commands, globalResource, cmdList) void
    }

    class RenderCommandList {
        - atomic~RenderTask*~ m_head
        - atomic~RenderTask*~ m_tail
        - LinearArena m_allocator[2]
        - atomic~size_t~ m_writeIndex
        - size_t m_readIndex
        + Enqueue(func) void
        + ExecuteAll(renderer, cmdList) void
        + Reset() void
    }

    class SceneProxy {
        - fvec3 m_position
        - fvec3 m_rotation
        - fvec3 m_scale
        + UpdatePosition(pos) void
        + UpdateRotation(rot) void
        + UpdateScale(scale) void
        + GetTransform() fmat4
    }

    class PrimitiveProxy {
        - RefPtr~const_MeshAsset~ m_refMesh
        - RefPtr~const_MaterialAsset~ m_refOverrideMaterial
        + SetMesh(refMesh) void
        + SetOverrideMaterial(refMaterial) void
        + GetMesh() RefPtr~const_MeshAsset~
    }

    class LightProxy {
    }

    class MeshBatch {
        - HashMap~UUID_TransformInfo~ m_transformInfos
        - RefPtr~RHIBuffer~ m_transformBuffer
        - RefPtr~MeshDrawCommand~ m_refDrawCommand
        - RefPtr~const_MeshAsset~ m_refMesh
        - RefPtr~const_MaterialAsset~ m_refMaterial
        - size_t m_sectionIndex
        + Upload(cmdList) void
        + Unload(cmdList) void
        + Sync(cmdList) void
        + AddTransform(id, transform) void
        + UpdateTransform(id, transform) void
        + RemoveTransform(id) void
        + GetInstanceCount() size_t
        + GetDrawCommand() RefPtr~MeshDrawCommand~
    }

    class MeshDrawCommand {
        + RefPtr~RHIVertexLayout~ vertexLayout
        + uint32_t indexOffset
        + uint32_t indexCount
        + HashMap~eTextureSlot_RefPtr_RHITexture~ textureSlots
        + HashMap~eVectorSlot_fvec3~ vectorValues
        + HashMap~eScalarSlot_float~ scalarValues
        + RefPtr~RHIBuffer~ instanceBuffer
        + uint32_t instanceCount
    }

    class MeshBatchKey {
        + UUID meshId
        + UUID materialId
        + size_t meshSection
        + operator==() bool
    }

    class UpdateProxyInfo {
        <<struct>>
        + UUID id
        + fvec3 position
        + fvec3 rotation
        + fvec3 scale
    }

    class RenderView {
        <<struct>>
        + fmat4 viewMatrix
        + fmat4 projectionMatrix
        + fvec3 cameraPosition
    }

    Renderer *-- RenderScene
    Renderer *-- RenderGraph
    Renderer *-- RenderCommandList

    RenderGraph *-- GlobalResource
    RenderGraph *-- PipeLine

    PipeLine <|-- SimpleColor

    RenderScene o-- PrimitiveProxy
    RenderScene o-- LightProxy
    RenderScene o-- MeshBatch

    SceneProxy <|-- PrimitiveProxy
    SceneProxy <|-- LightProxy

    MeshBatch *-- MeshDrawCommand
    MeshBatch ..> MeshBatchKey : keyed by
```

---

### 4. RHI — Render Hardware Interface

References Unreal Engine's `FRHICommandList`, `FRHIResource`, and `FRHICommandListExecutor` patterns. Abstracts the actual GPU API (OpenGL / DirectX / Vulkan), and all GPU commands are encapsulated as **Command Objects** before being executed serially.

```mermaid
classDiagram
    class RHISystem {
        <<abstract>>
        + Init(nativeHandle) bool
        + Clear(state) void
        + Flush() void
        + Present() void
        + Resize(...) void
        + SetColorState(state) void
        + SetDepthState(state) void
        + SetStencilState(state) void
        + SetBlendState(state) void
        + SetRasterizerState(state) void
        + CreateBuffer(desc) RefPtr~RHIBuffer~
        + CreateVertexLayout(desc) RefPtr~RHIVertexLayout~
        + CreateTexture(desc) RefPtr~RHITexture~
        + CreateSampler(desc) RefPtr~RHISampler~
        + CreateVertexShader(desc) RefPtr~RHIShader~
        + CreatePixelShader(desc) RefPtr~RHIShader~
        + CreatePipeLine(desc) RefPtr~RHIPipeLine~
        + InitializeBuffer(info, buffer) void
        + UpdateBuffer(info, buffer) void
        + DrawIndexPrimitive(info) void
        + DispatchCompute(info) void
    }

    class GLSystem {
    }
    note for GLSystem "OpenGL 4.5 implementation"

    class RHIResource {
        - atomic~eResourceState~ m_state
        + GetRawBuffer() void*
        + SetState(eState) void
        + GetState() eResourceState
    }

    class RHIBuffer {
        - RHIBufferDesc m_desc
        + GetBufferType() eBufferType
        + GetAccessType() eDataAccess
        + GetSize() uint32_t
        + GetStride() uint32_t
        + GetCount() uint32_t
    }

    class RHIVertexLayout {
        - RHIVertexLayoutDesc m_desc
        + GetNumAttributes() size_t
    }

    class RHITexture {
        - RHITextureDesc m_desc
        + GetWidth() uint32_t
        + GetHeight() uint32_t
        + GetPixelFormat() ePixelFormat
        + GetTextureType() eTextureType
    }

    class RHISampler {
        - RHISamplerDesc m_desc
        + GetMinFilter() eFilterMode
        + GetMagFilter() eFilterMode
    }

    class RHIShader {
        - RHIShaderDesc m_desc
        + GetShaderType() eShaderType
    }

    class RHIPipeLine {
        - RHIPipeLineDesc m_desc
        - HashMap~string_RHIResourceBinding~ m_slots
        + GetClearState() RHIClearState
        + GetDepthState() RHIDepthState
        + AddSlot(name, binding) void
        + HasSlot(name) bool
        + GetBindingSlot(name) RHIResourceBinding
    }

    class RHICommandList {
        - RefPtr~RHISystem~ m_system
        - DynamicArray~RHICommandBase*~ m_commands
        - size_t m_frame
        + Enqueue~T~(...) void
        + ExecuteAll() void
        + Reset() void
        + SetFrame(frame) void
        + CreateBuffer(info) RefPtr~RHIBuffer~
        + InitializeBuffer(info, buffer) void
        + UpdateBuffer(info, buffer) void
        + DrawIndexPrimitive(info) void
    }

    class RHICommandBase {
        - ExecuteFunc m_func
        + Execute(system) void
    }

    class RHICommandT {
        <<RHICommand~T~>>
        + ExecuteAndDestruct(system, base) static void
    }
    
    note for RHICommandT "45+ command classes:\nRHICommandClear\nRHICommandFlush\nRHICommandPresent\nRHICommandInitializeBuffer\nRHICommandUpdateBuffer\n..."

    class RHIExecutor {
        <<abstract>>
        - RefPtr~RHISystem~ m_system
        + Acquire() RefPtr~RHICommandList~
        + Submit(cmdList) void
        + Execute() void
    }

    class RHIFrameExecutor {
        - size_t m_frameCount
        - size_t m_recordIndex
        - size_t m_beginIndex
        - atomic~size_t~ m_endIndex
        - DynamicArray~RefPtr_RHICommandList~ m_listPool
        + Acquire() RefPtr~RHICommandList~
        + Submit(cmdList) void
        + Execute() void
        + Init(frameCount) bool
    }

    class RHITaskExecutor {
        + Acquire() RefPtr~RHICommandList~
        + Submit(cmdList) void
        + Execute() void
    }
    note for RHITaskExecutor "Command list pool for asset/task uploads"

    RHISystem <|-- GLSystem

    RHIResource <|-- RHIBuffer
    RHIResource <|-- RHIVertexLayout
    RHIResource <|-- RHITexture
    RHIResource <|-- RHISampler
    RHIResource <|-- RHIShader
    RHIResource <|-- RHIPipeLine

    RHICommandBase <|-- RHICommandT

    RHICommandList o-- RHICommandBase

    RHIExecutor <|-- RHIFrameExecutor
    RHIExecutor <|-- RHITaskExecutor

    RHICommandList ..> RHISystem : delegates to
```

---

### 5. Asset — Async Asset Loading System

Asset loading is organized as a 3-stage pipeline: **Parse (file read)** → **Load (GPU format conversion)** → **Upload (GPU upload)**. Each stage runs in a different thread context, ensuring the main loop is never blocked.

```mermaid
classDiagram
    class Asset {
        <<ECS::Object>>
        + UUID id
        + string name
        + const string path
        + const eExtension extension
        + const eAsset type
        - atomic~eAssetState~ m_state
        + GetResourceState() eResourceState
        + SetState(state) void
        + GetState() eAssetState
    }

    class MeshAsset {
        + HashMap~VertexKey_RefPtr_FormattedBuffer~ rawBuffers
        + HashMap~VertexKey_RefPtr_RHIBuffer~ buffers
        + RefPtr~FormattedBuffer~ rawIndex
        + RefPtr~RHIBuffer~ index
        + DynamicArray~MeshSection~ sections
        + HashMap~string_RefPtr_MaterialAsset~ materials
        + GetResourceState() eResourceState
    }

    class MaterialAsset {
        + HashMap~eTextureSlot_RefPtr_TextureAsset~ textures
        + HashMap~eVectorSlot_fvec3~ vectorValues
        + HashMap~eScalarSlot_float~ scalarValues
        + GetResourceState() eResourceState
    }

    class TextureAsset {
        + RefPtr~FormattedBuffer~ rawBuffer
        + RefPtr~RHITexture~ texture
        + uint32_t width
        + uint32_t height
        + ePixelFormat pixelFormat
        + GetResourceState() eResourceState
    }

    class ShaderAsset {
        + RefPtr~RawBuffer~ rawBuffer
        + RefPtr~RHIShader~ shader
        - eShaderType m_shaderType
        + GetResourceState() eResourceState
        + SetShaderType(type) void
    }

    class AssetSystem {
        + Load(path) static RefPtr~Asset~
        + Unload(path) static void
        + Shutdown() static void
        + GetTask() static TaskQueue
        + AddTask(asset) static void
        + Release(cmdList) static void
        + GetParser(path) static RefPtr~AssetParser~
    }

    class AssetFactory {
        + Create(path) RefPtr~Asset~
        + Create(path, extension) RefPtr~Asset~
        + GetExtension(path) eExtension
    }

    class AssetParser {
        <<abstract>>
        + Parse(asset) void
        + Load(asset, cmdList) void
        + Unload(asset, cmdList) void
    }

    class OBJParser {
        + Parse(asset) void
        + Load(asset, cmdList) void
    }

    class MTLParser {
        + Parse(asset) void
        + Load(asset, cmdList) void
    }

    class GLSLParser {
        + Parse(asset) void
        + Load(asset, cmdList) void
    }

    class StbImageParser {
        + Parse(asset) void
        + Load(asset, cmdList) void
    }

    class MeshSection {
        <<struct>>
        + uint32_t indexOffset
        + uint32_t indexCount
        + uint32_t minVertexIndex
        + uint32_t maxVertexIndex
        + string materialName
        + string name
    }

    Asset <|-- MeshAsset
    Asset <|-- MaterialAsset
    Asset <|-- TextureAsset
    Asset <|-- ShaderAsset

    MeshAsset *-- MeshSection
    MeshAsset o-- MaterialAsset

    AssetParser <|-- OBJParser
    AssetParser <|-- MTLParser
    AssetParser <|-- GLSLParser
    AssetParser <|-- StbImageParser

    AssetSystem ..> AssetFactory : uses
    AssetSystem ..> AssetParser : uses
    AssetFactory ..> Asset : creates
```

---

## Threading and Data Flow

### Thread Overview

The engine consists of 4 independent worker threads and 1 task thread pool.

```mermaid
flowchart TD
    main["Main Thread<br>Window Event Loop<br>+ Input Polling"]
    world["WorldWorker<br>(Logic Thread)<br>ECS Update<br>Commander<br>Fill RenderCommandList"]
    render["RenderWorker<br>(Render Thread)<br>RenderScene Flush<br>MeshBatch Sync<br>RenderGraph Execute<br>Fill RHICommandList"]
    rhi["RHIWorker<br>(GPU Submit Thread)<br>RHIFrameExecutor Execute<br>RHITaskExecutor Execute<br>OpenGL / GPU API calls"]
    asset["AssetWorker<br>(Asset Thread)<br>Consume AssetSystem Queue<br>Dispatch to Parsers"]
    task["TaskWorker Pool<br>(N threads)<br>OBJ / PNG / GLSL<br>File parsing (CPU-bound)"]

    main -->|InputStorage| world
    world -->|FrameGate updateGate<br>+ RenderCommandList| render
    render -->|FrameGate renderGate<br>+ RHICommandList Frame| rhi
    asset -->|RHICommandList Task<br>GPU upload| rhi
    asset -->|Dispatch parse tasks| task
```

---

### Frame Update Sequence — World → Renderer → RHI

`FrameGate` implements producer-consumer synchronization between World and Renderer, and between Renderer and RHI.

```mermaid
sequenceDiagram
    participant WW as WorldWorker<br>(Logic Thread)
    participant FG1 as FrameGate<br>(updateGate)
    participant RCL as RenderCommandList
    participant RW as RenderWorker<br>(Render Thread)
    participant FG2 as FrameGate<br>(renderGate)
    participant RHICL as RHICommandList<br>(Frame)
    participant RHIW as RHIWorker<br>(GPU Submit)
    participant GPU as GPU (OpenGL)

    Note over WW,GPU: == Frame N Start ==
    WW->>WW: ECS Update<br>(Systems, Components)
    WW->>RCL: Commander.AddPrimitive()<br>Commander.Update()<br>Commander.SetView()
    Note right of RCL: RenderTask lambdas stored<br>in LinearArena<br>(lock-free queue)
    WW->>FG1: Submit()<br>(m_frameDiff++)
    Note right of FG1: condition_variable<br>wakes RenderWorker

    Note over WW,GPU: == Render Thread Processing ==
    FG1->>RW: Acquire() done<br>(m_frameDiff--)
    RW->>RHICL: Executor.Acquire()<br>(acquire next frame command list)
    RW->>RCL: ExecuteAll(renderer, cmdList)<br>(execute all RenderTasks)
    Note right of RCL: Inside each task:<br>RenderScene.AddBatch()<br>RenderScene.UpdateBatch()<br>RenderScene.Flush()<br>GlobalResource.UpdateCamera()
    RW->>RHICL: RenderGraph.Execute()<br>(PipeLine.Draw() -> generate draw commands)
    RW->>RHICL: Enqueue<RHICommandDraw>()
    RW->>RHICL: Executor.Submit(cmdList)
    RW->>FG2: Submit()<br>(m_frameDiff++)

    Note over WW,GPU: == RHI Thread Processing ==
    FG2->>RHIW: Acquire() done
    RHIW->>RHICL: FrameExecutor.Execute()<br>(consume command lists)
    RHICL->>GPU: RHICommand::Execute(RHISystem)<br>(OpenGL API calls)
    GPU->>GPU: DrawIndexPrimitive<br>Present
    RHIW->>RHIW: TaskExecutor.Execute()<br>(process asset upload commands)
```

---

### Asset Loading Sequence — Async 3-Stage Pipeline

```mermaid
sequenceDiagram
    participant GC as Game Code<br>(Main Thread)
    participant AS as AssetSystem
    participant AW as AssetWorker<br>(Asset Thread)
    participant TW as TaskWorker Pool<br>(Parse Threads)
    participant RTE as RHITaskExecutor<br>(Task CmdList)
    participant RHIW as RHIWorker<br>(GPU Submit)
    participant GPU as GPU

    Note over GC,GPU: == Asset Load Request ==
    GC->>AS: AssetSystem::Load("mesh.obj")
    AS->>AS: AssetFactory::Create(path)<br>-> MeshAsset created<br>(state = eLoaded)
    AS->>AS: Enqueue(asset) to internal queue

    Note over GC,GPU: == Parse Stage (CPU-bound) ==
    AW->>AS: GetTask() -> dequeue MeshAsset
    AW->>TW: TaskWorker::Set(ParseTask)<br>(condition_variable notify)
    TW->>TW: OBJParser::Parse(asset)<br>read file -> fill rawBuffers<br>(state = eParsed)
    TW-->>AW: completion signal

    Note over GC,GPU: == Load Stage (GPU format conversion) ==
    AW->>RTE: Executor.Acquire()<br>-> acquire RHICommandList
    AW->>RTE: OBJParser::Load(asset, cmdList)<br>-> Enqueue<RHICommandInitializeBuffer>()<br>-> Enqueue<RHICommandInitializeTexture>()
    AW->>RTE: Executor.Submit(cmdList)

    Note over GC,GPU: == GPU Upload Stage ==
    RHIW->>RTE: TaskExecutor.Execute()
    RTE->>GPU: RHICommand::Execute(RHISystem)<br>-> glBufferData / glTexImage2D
    GPU-->>RHIW: done
    Note right of RHIW: asset.state = eReady<br>MeshBatch can now<br>reference RHIBuffer
```

---

### Synchronization Mechanism Detail

```mermaid
classDiagram
    class FrameGate {
        - condition_variable m_cvProducer
        - condition_variable m_cvConsumer
        - mutex m_mutex
        - const size_t m_frameLimit
        - atomic~int32_t~ m_frameDiff
        - atomic~bool~ m_notified
        + Submit()
        + Acquire()
    }
    note for FrameGate "Submit() {\n  lock(m_mutex)\n  m_frameDiff++\n  m_cvConsumer.notify_one()\n}\n\nAcquire() {\n  lock(m_mutex)\n  m_cvConsumer.wait(\n    lock, [&]{ return m_frameDiff > 0 }\n  )\n  m_frameDiff--\n}"

    class RHIFrameExecutor {
        - RHICommandList[] m_listPool
        - size_t m_recordIndex
        - size_t m_beginIndex
        - atomic m_endIndex
        + Acquire()
        + Submit()
        + Execute()
    }
    note for RHIFrameExecutor "m_recordIndex <- written by RenderWorker\nm_beginIndex <- read start index for RHIWorker\nm_endIndex <- cross-thread: Submit writes, Execute reads\n\nAcquire() -> returns m_listPool[m_recordIndex]\nSubmit() -> advances m_recordIndex, increments m_endIndex\nExecute() -> processes range m_beginIndex ~ m_endIndex"

    class RenderCommandList {
        - atomic~RenderTask*~ m_head
        - atomic~RenderTask*~ m_tail
        - LinearArena m_allocator[2]
        - atomic~size_t~ m_writeIndex
        + Enqueue()
        + ExecuteAll()
    }
    note for RenderCommandList "m_allocator <- ping-pong buffers\n\nEnqueue() {\n  task = m_allocator[writeIdx].Alloc()\n  CAS(m_tail, task) <- lock-free insert\n}\nExecuteAll() {\n  m_writeIndex ^= 1 <- swap buffers\n  while(head) head->Execute(); head = head->next\n}"
```

---

### Per-Worker Activity Diagrams

Control flow for each Worker during one frame or one task.

#### WorldWorker (Logic Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart()<br>WorldContext.Init()<br>Connect InputStorage"]
    Init --> Loop{"m_isRunning? (true)"}
    Loop -->|true| Swap["InputStorage.SwapInput()<br>Atomic swap: CurrData -> PrevData"]
    Swap --> Prepare["WorldContext.Prepare()<br>RenderCommandList.Reset()"]
    Prepare --> Update["WorldContext.Update(timeStep)<br>Execute ECS Systems in order<br>(CameraSystem / MoveSystem / ...)"]
    Update --> Note1["Note: During System execution, Commander<br>enqueues Scene changes into<br>RenderCommandList as RenderTask lambdas"] -.-> Update
    Update --> Submit["updateGate.Submit()<br>m_frameDiff++ -> wake RenderWorker"]
    Submit --> Loop
    Loop -->|false| Destroy["onDestroy()"]
    Destroy --> Stop([Stop])
```

#### RenderWorker (Render Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart()"]
    Init --> Loop{"m_isRunning? (true)"}
    Loop -->|true| Acquire1["updateGate.Acquire()<br>Wait for WorldWorker Submit<br>(until m_frameDiff > 0)"]
    Acquire1 --> Acquire2["RHIFrameExecutor.Acquire()<br>Acquire RHICommandList from triple buffer<br>Advance m_recordIndex"]
    Acquire2 --> Execute["RenderCommandList.ExecuteAll(renderer, cmdList)<br>Execute all RenderTasks<br>-> RenderScene Flush / MeshBatch Sync"]
    Execute --> UpdateCam["GlobalResource.UpdateCamera(renderView, cmdList)<br>Upload View & Proj matrices to UBO"]
    UpdateCam --> Graph["RenderGraph.Execute(cmdList, renderScene, renderView)<br>PipeLine.Draw()<br>-> Enqueue RHICommandDrawIndexPrimitive"]
    Graph --> SubmitRHI["RHIFrameExecutor.Submit(cmdList)<br>Atomic increment m_endIndex<br>-> RHIWorker can now read"]
    SubmitRHI --> SubmitGate["renderGate.Submit()<br>m_frameDiff++ -> wake RHIWorker"]
    SubmitGate --> Loop
    Loop -->|false| Destroy["onDestroy()"]
    Destroy --> Stop([Stop])
```

#### RHIWorker (GPU Submit Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart()<br>RHISystem.Init()<br>Initialize WGLContext (create OpenGL context)"]
    Init --> Loop{"m_isRunning? (true)"}
    Loop -->|true| Acquire["renderGate.Acquire()<br>Wait for RenderWorker Submit"]
    Acquire --> ExecuteFrame["RHIFrameExecutor.Execute()<br>Consume range m_beginIndex ~ m_endIndex sequentially"]
    ExecuteFrame --> Note1["Note: For each RHICommandList.ExecuteAll():<br>RHICommandBase::Execute(RHISystem)<br>-> direct OpenGL API calls<br>After loop: RHISystem.Present()"] -.-> ExecuteFrame
    ExecuteFrame --> ExecuteTask["RHITaskExecutor.Execute()<br>Process asset upload commands<br>glBufferData / glTexImage2D"]
    ExecuteTask --> Loop
    Loop -->|false| Destroy["onDestroy()"]
    Destroy --> Stop([Stop])
```

#### AssetWorker (Asset Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart()<br>Initialize TaskWorker pool (N threads)"]
    Init --> Loop{"m_isRunning? (true)"}
    Loop -->|true| Check["AssetSystem.GetTask()<br>Check for pending assets"]
    Check --> Cond{"pending asset?"}
    
    Cond -->|yes| Fork1["TaskWorker A<br>OBJParser.Parse(asset)<br>Fill rawBuffers<br>state = eParsed"]
    Cond -->|yes| Fork2["TaskWorker B<br>StbImageParser.Parse(asset)<br>Fill rawBuffer<br>state = eParsed"]
    
    Fork1 --> Merge
    Fork2 --> Merge
    
    Merge["Merge"] --> Acquire["RHITaskExecutor.Acquire()<br>Acquire RHICommandList"]
    Acquire --> Load["Parser.Load(asset, cmdList)<br>GPU format conversion<br>Enqueue RHICommandInitializeBuffer<br>Enqueue RHICommandInitializeTexture"]
    Load --> Submit["RHITaskExecutor.Submit(cmdList)<br>state = eReady"]
    Submit --> Note1["Note: RHIWorker later calls TaskExecutor.Execute()<br>to perform actual GPU upload"] -.-> Submit
    Submit --> Loop
    
    Cond -->|no| Idle["yield / idle"]
    Idle --> Loop
    
    Loop -->|false| Destroy["onDestroy()"]
    Destroy --> Stop([Stop])
```

---

## World → Renderer Data Flow Detail

```mermaid
flowchart TD
    subgraph WorldLayer["World Layer"]
        Entity[Entity]
        SceneComponent["SceneComponent<br>(position/rotation/scale)"]
        MeshComponent["MeshComponent<br>(MeshAsset ref)"]
        MaterialComponent["MaterialComponent<br>(MaterialAsset ref)"]
        Scene[Scene]
        Commander[Commander]

        Entity --> SceneComponent
        Entity --> MeshComponent
        Entity --> MaterialComponent
        SceneComponent --> Scene
        Scene --> Commander
    end

    subgraph BridgeLayer["Bridge Layer"]
        RenderCommandList["RenderCommandList<br>(RenderTask lambda queue)"]
        BridgeNote["Note: WorldWorker writes (Producer)<br>RenderWorker reads (Consumer)<br>Zero-alloc via LinearArena ping-pong"] -.- RenderCommandList
    end

    subgraph RendererLayer["Renderer Layer"]
        RenderScene[RenderScene]
        PrimitiveProxy["PrimitiveProxy<br>(SceneProxy + Mesh/Material)"]
        MeshBatch["MeshBatch<br>(grouped by Mesh+Material+Section key)"]
        MeshDrawCommand["MeshDrawCommand<br>(VBO, IBO, instance buffer, texture slots)"]
        RenderGraph[RenderGraph]
        PipeLine["PipeLine<br>(SimpleColor / Deferred planned)"]

        RenderScene --> PrimitiveProxy
        PrimitiveProxy --> MeshBatch
        MeshBatch --> MeshDrawCommand
        MeshDrawCommand --> RenderGraph
        RenderGraph --> PipeLine
    end

    subgraph RHILayer["RHI Layer"]
        RHICommandList["RHICommandList<br>(GPU command object queue)"]
        RHISystem["RHISystem<br>(GLSystem)"]
        GPU["GPU<br>(OpenGL 4.5)"]

        PipeLine --> RHICommandList
        RHICommandList --> RHISystem
        RHISystem --> GPU
    end

    Commander --> RenderCommandList
    RenderCommandList --> RenderScene
```

---

## Development Process and Technical Challenges

### 1. Multi-threaded Pipeline Design

* **Problem** : Frame data needed to be transferred between World logic updates and GPU rendering without data races
* **Solution** : Two `FrameGate` instances (updateGate, renderGate) handle World→Renderer and Renderer→RHI synchronization independently. `RenderCommandList` uses a lock-free atomic pointer queue with a ping-pong LinearArena, allowing World thread Enqueue and Render thread ExecuteAll to operate safely without locks

### 2. MeshBatch Instancing

* **Problem** : Submitting dozens to hundreds of objects with the same Mesh + Material combination as individual draw calls causes CPU overhead to spike
* **Solution** : A HashMap keyed by `MeshBatchKey (meshId, materialId, sectionIndex)` is maintained in `RenderScene`. Objects with the same key are grouped into a single `MeshBatch`, and transform data is batch-uploaded to the GPU as an instance buffer (`RHIBuffer`)

### 3. RHI Command Object Pattern

* **Problem** : Calling OpenGL APIs directly from the render thread causes `GLContext` thread affinity issues and makes API abstraction impossible
* **Solution** : All GPU commands are encapsulated as `RHICommandBase`-derived objects (45+). RenderWorker only enqueues command objects into `RHICommandList`, and RHIWorker exclusively calls `ExecuteAll()` to perform actual OpenGL calls. `RHIFrameExecutor` uses triple buffering to prevent CPU-GPU pipeline stalls

### 4. PrimitiveProxy Pattern

* **Problem** : Exposing World Entity/Component directly to the renderer creates tight coupling between World and Renderer, hindering independent evolution of both systems
* **Solution** : References Unreal Engine's `UPrimitiveComponent` → `FPrimitiveSceneProxy` pattern. When a `MeshNode` attaches to the Scene, a `PrimitiveProxy` is created. Transform changes are forwarded to the Renderer via `Commander` as `UpdateProxyInfo`. The Renderer has zero knowledge of World types

### 5. Async 3-Stage Asset Pipeline

* **Problem** : Loading large OBJ/PNG files blocks the main loop, and GPU uploads must run exclusively in the RHI thread context
* **Solution** : Separated into 3 stages: Parse (TaskWorker pool, parallel CPU) → Load (AssetWorker, GPU format conversion) → Upload (RHITaskExecutor, GPU upload). `atomic<eAssetState>` tracks state transitions safely, and MeshBatch only references `RHIBuffer` once asset state is `eReady`

### 6. Zero-Allocation Render Command Queue

* **Problem** : Allocating RenderTasks with `new` every frame accumulates heap fragmentation and allocation overhead
* **Solution** : `RenderCommandList` runs two `LinearArena` instances in a ping-pong fashion. The write index is atomically swapped so the Producer (World) writes to one buffer while the Consumer (Renderer) drains the other — a Zero-Alloc design

---

## Implementation Status

| System | Status |
|--------|--------|
| ECS (Entity, Component, Node, System, Graph) | ✅ Complete |
| Memory (Pool/Array allocators, RefPtr/ObjectPtr) | ✅ Complete |
| Runtime Reflection (TypeInfo, PropertyInfo, MethodInfo) | ✅ Complete |
| Container (Vector, HashMap, HashSet) | ✅ Complete |
| RHI Layer (OpenGL 4.5, WGL) | ✅ Basic implementation |
| RHI Command Object pattern | ✅ Complete |
| RHI Triple Buffering (FrameExecutor) | ✅ Complete |
| World / Commander / Scene | ✅ Complete |
| PrimitiveProxy / LightProxy | ✅ Complete |
| MeshBatch instancing | ✅ Complete |
| RenderGraph / PipeLine | ✅ Basic implementation (SimpleColor) |
| GlobalResource (Camera, GBuffer textures) | ✅ Complete |
| Asset System (OBJ/MTL/PNG/GLSL) | ✅ Complete |
| Async Asset loading pipeline | ✅ Complete |
| Multi-threaded Worker + FrameGate | ✅ Complete |
| Input System | 🚧 Planned |
| Deferred Rendering Pipeline | 🚧 Planned |
| GC (Mark & Sweep) full integration | 🚧 In progress |
| DirectX 11 / 12 RHI backend | 🚧 Planned |
| Serialization (Reflection-based JSON/Binary) | 🚧 Planned |
| Physics engine integration | 🚧 Planned |

---

## Build Instructions

### Requirements

* **OS:** Windows 10/11 (64-bit)
* **Compiler:** MSVC (Visual Studio 2019+), C++17
* **Tools:** CMake 3.15+, Git

### Clone and Build

```bash
# Clone with submodules
git clone --recursive [https://github.com/YourUsername/GameEngine.git](https://github.com/YourUsername/GameEngine.git)
cd GameEngine

# If cloned without submodules
git submodule update --init --recursive

# CMake build
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## Tech Stack

| Item | Details |
|------|---------|
| Language | C++17 |
| Graphics API | OpenGL 4.5 Core Profile |
| Windowing | WGL (Win32 API) |
| Math | GLM 1.0.1 |
| Build | CMake 3.15+ |
| Platform | Windows 10/11 |

---

## License

This project is distributed under the MIT License.