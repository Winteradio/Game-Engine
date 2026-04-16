# Custom Game Engine

## Project Overview

This project is a **C++17 Data-Oriented game engine built from scratch**, referencing the architecture of **Unreal Engine**.

The goal is to design and implement the core structures found in commercial engines: **Runtime Reflection**, **Garbage Collection (Mark & Sweep)**, **ECS (Entity Component System)**, and the key rendering pipeline concepts of **MeshBatch**, **PrimitiveProxy**, **RenderGraph**, and **RHI (Render Hardware Interface)**.

Every core layer of the engine is directly controlled — from STL-free custom containers and purpose-built memory allocators to a multi-threaded worker-based pipeline — pursuing a complete understanding of both performance and architecture.

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

```plantuml
@startuml External_Component_Diagram

skinparam componentStyle rectangle
skinparam BackgroundColor #FAFAFA
skinparam ComponentBorderColor #555
skinparam ArrowColor #444
skinparam packageStyle frame

package "External Modules (Git Submodules)" {
    [ECS]
    [Memory]
    [Reflection]
    [Container]
    [Log]
    [glm]
    [glad]
    [imgui]
    [yaml-cpp]
    [stb]
}

package "Game Engine" {
    [Framework]
    [World]
    [Renderer]
    [RHI]
    [Asset]
}

ECS         --> World       : Entity / Component\nNode / System / Graph
Reflection  --> World       : GENERATE / PROPERTY / METHOD\nRuntime type registration
glm         --> World       : fvec3 / fmat4 math operations
glm         --> Renderer    : Matrix / vector operations
glm         --> RHI         : RHIDescriptions math types
glad        --> RHI         : OpenGL 4.5 Core Profile\nAPI loading (WGL)
imgui       --> Framework   : Debug GUI rendering
yaml-cpp    --> Asset       : .yaml config file parsing
stb         --> Asset       : stb_image\nImage byte decoding

Memory      ..> Framework   : RefPtr / ObjectPtr / RootPtr\nPool & Array allocators
Memory      ..> World
Memory      ..> Renderer
Memory      ..> RHI
Memory      ..> Asset

Container   ..> Framework   : DynamicArray / HashMap\nHashSet / StaticArray
Container   ..> World
Container   ..> Renderer
Container   ..> RHI
Container   ..> Asset

Log         ..> Framework   : LOGINFO / LOGERROR macros
Log         ..> World
Log         ..> Renderer
Log         ..> RHI
Log         ..> Asset

@enduml
```

---

### 0-B. Game Engine Internal Modules — Component Dependency

Dependency directions and responsibilities of the five internal engine modules. Dependencies always flow **toward lower layers only**.

```plantuml
@startuml Internal_Component_Diagram

skinparam componentStyle rectangle
skinparam BackgroundColor #FAFAFA
skinparam ComponentBorderColor #555
skinparam ArrowColor #444

component "Framework\n─────────────────\nEngine · Window\nWorker · FrameGate\nInput · Task · Application" as FW

component "World\n─────────────────\nECS wrapper layer\nEntity · Component · Node\nSystem · Commander\nScene · WorldContext" as WO

component "Renderer\n─────────────────\nPrimitiveProxy · LightProxy\nRenderScene · MeshBatch\nRenderGraph · PipeLine\nGlobalResource" as RD

component "RHI\n─────────────────\nRHISystem (GLSystem)\nRHIResources · RHICommandList\nRHIFrameExecutor · RHITaskExecutor\nRHI Command Objects (45+)" as RI

component "Asset\n─────────────────\nAssetSystem · AssetFactory\nOBJ / MTL / PNG / GLSL Parsers\nAsync 3-stage pipeline" as AS

WO  -->  FW : InputStorage (read input)\nFrameGate (frame sync participation)
RD  -->  WO : RenderCommandList (consume)\nRenderScene <- Commander Enqueue
RD  -->  RI : RHICommandList (generate GPU commands)\nRHIFrameExecutor.Acquire / Submit
AS  -->  RI : RHITaskExecutor\n(submit GPU upload commands)
FW  -->  RI : RHIExecutor lifecycle management\n(Init / Shutdown)
FW  -->  WO : WorldContext lifecycle management
FW  -->  RD : Renderer lifecycle management
FW  -->  AS : AssetWorker & TaskWorker management

note bottom of FW
  Owns and controls all Worker threads.
  FrameGate synchronizes the
  World -> Renderer -> RHI frame flow.
end note

note bottom of RI
  Actual GPU API calls happen only in RHIWorker.
  RHISystem is a platform abstraction interface
  (swappable: OpenGL / DirectX / Vulkan).
end note

@enduml
```

---

### 1. Engine — Full System Composition

The `Engine` class owns and initializes all workers (threads) and subsystems. `FrameGate` synchronizes the frame flow in World → Renderer → RHI order.

```plantuml
@startuml Engine_ClassDiagram

skinparam classAttributeIconSize 0
skinparam BackgroundColor #FAFAFA
skinparam ClassBorderColor #555
skinparam ArrowColor #333

package "Framework" {

    class Engine {
        - m_window : RefPtr<Window>
        - m_inputHandler : RefPtr<InputHandler>
        - m_inputStorage : RefPtr<InputStorage>
        - m_updateGate : RefPtr<FrameGate>
        - m_renderGate : RefPtr<FrameGate>
        - m_worldContext : RefPtr<WorldContext>
        - m_renderer : RefPtr<Renderer>
        - m_rhiSystem : RefPtr<RHISystem>
        - m_rhiFrameExecutor : RefPtr<RHIExecutor>
        - m_rhiTaskExecutor : RefPtr<RHIExecutor>
        - m_worldWorker : RefPtr<Worker>
        - m_renderWorker : RefPtr<Worker>
        - m_rhiWorker : RefPtr<Worker>
        - m_assetWorker : RefPtr<Worker>
        + Init(windowDesc, renderDesc) : bool
        + Shutdown() : void
        + Run() : void
        + GetInputStorage() : RefPtr<InputStorage>
        + GetWorldContext() : RefPtr<WorldContext>
        + GetRenderer() : RefPtr<Renderer>
        - InitWindow() : bool
        - InitRender() : bool
        - InitRHI() : bool
        - InitWorld() : bool
        - InitAsset() : bool
        - InitWorker() : bool
        - UpdateInput() : void
    }

    abstract class Worker {
        - m_isRunning : atomic<bool>
        - m_thread : std::thread
        + Start() : void
        + Stop() : void
        # onStart() : virtual bool
        # onUpdate() : virtual void
        # onDestroy() : virtual void
        # onNotify() : virtual void
        - Run() : void
    }

    class TaskWorker {
        - m_isWaited : atomic<bool>
        - m_cv : condition_variable
        - m_mutex : mutex
        - m_task : RefPtr<DefaultTask>
        + Wait() : void
        + Notify() : void
        + Set(task) : void
        + IsWaited() : bool
    }

    abstract class FrameHandler {
        + NotifyAll() : virtual void
    }

    abstract class FrameProducer {
        + Submit() : virtual void
    }

    abstract class FrameConsumer {
        + Acquire() : virtual void
    }

    class FrameGate {
        - m_cvProducer : condition_variable
        - m_cvConsumer : condition_variable
        - m_mutex : mutex
        - m_frameLimit : const size_t
        - m_frameDiff : atomic<int32_t>
        - m_notified : atomic<bool>
        + Submit() : void
        + Acquire() : void
        + NotifyAll() : void
    }

    class WorldWorker {
        - m_timeStep : ECS::TimeStep
        - m_refInputStorage : RefPtr<InputStorage>
        - m_refWorldContext : RefPtr<WorldContext>
        - m_refProducer : RefPtr<FrameProducer>
        - m_renderViews : DynamicArray<RenderView>
    }

    class RenderWorker {
        - m_refRenderer : RefPtr<Renderer>
        - m_refExecutor : RefPtr<RHIExecutor>
        - m_refConsumer : RefPtr<FrameConsumer>
        - m_refProducer : RefPtr<FrameProducer>
    }

    class RHIWorker {
        - m_refSystem : RefPtr<RHISystem>
        - m_refFrameExecutor : RefPtr<RHIExecutor>
        - m_refTaskExecutor : RefPtr<RHIExecutor>
        - m_refConsumer : RefPtr<FrameConsumer>
    }

    class AssetWorker {
        - m_threads : DynamicArray<RefPtr<TaskWorker>>
        - m_refTaskExecutor : RefPtr<RHIExecutor>
        + SetTaskThread(count) : void
        + SetTaskExecutor(executor) : void
    }
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

Engine *-- FrameGate : m_updateGate\nm_renderGate
Engine *-- WorldWorker
Engine *-- RenderWorker
Engine *-- RHIWorker
Engine *-- AssetWorker

AssetWorker *-- TaskWorker

@enduml
```

---

### 2. World — ECS Game Logic Layer

References Unreal Engine's `UWorld`, `AActor`, `UActorComponent` structure. `Entity` acts as the `Actor`, `Component` holds data, and `Node` handles component composition. `Commander` is the one-way data bridge from World → Renderer.

```plantuml
@startuml World_ClassDiagram

skinparam classAttributeIconSize 0
skinparam BackgroundColor #FAFAFA

package "World" {

    class WorldContext {
        + world : RootPtr<World>
        + views : RefPtr<ViewController>
        + players : RefPtr<PlayerController>
        + commander : RefPtr<Commander>
        - m_refCommandList : RefPtr<WorldCommandList>
        + Init() : bool
        + Clear() : void
        + Prepare() : void
        + Update(timeStep) : void
    }

    class World <<ECS::Object>> {
        + scene : Scene
        - m_entityStorage : ObjectStorage<Entity>
        - m_componentContainer : Registry<BaseComponent>
        - m_nodeContainer : Registry<BaseNode>
        - m_systemRegistry : Graph<BaseSystem>
        + Init(refCommander) : bool
        + Update(timeStep) : void
        + CreateEntity() : ObjectPtr<Entity>
        + CreateComponent<T>(...) : ObjectPtr<T>
        + CreateNode<T>(...) : ObjectPtr<T>
        + CreateSystem<T>(...) : RefPtr<T>
        + GetEntity(uuid) : ObjectPtr<Entity>
        + RemoveEntity(uuid) : void
    }

    class Entity <<ECS::Entity>> {
        - m_owner : World*
        + Clear() : void
        + AddComponent<T>(...) : bool
        + AddNode<T>(...) : bool
        + GetComponent<T>() : ObjectPtr<T>
        + GetNode<T>() : ObjectPtr<T>
        + HasAllComponents<TList>() : bool
    }

    class Scene {
        - m_refCommander : RefPtr<Commander>
        - m_sceneDatas : HashMap<UUID, ScenePair>
        + Attach(node) : void
        + Detach(node) : void
        + Detach(entityId) : void
        + Update(entityId) : void
    }

    class Commander {
        - m_refCmdList : RefPtr<RenderCommandList>
        + SetView(renderView) : void
        + AddPrimitive(meshNode) : void
        + RemovePrimitive(meshNode) : void
        + AddLight(lightNode) : void
        + RemoveLight(lightNode) : void
        + Update(sceneComponent) : void
        + Remove(entityId) : void
        + RemoveAll() : void
    }

    abstract class BaseComponent <<ECS::Component>> {
    }

    class SceneComponent {
        - m_position : fvec3
        - m_rotation : fvec3
        - m_scale : fvec3
        - m_scene : Scene*
        + UpdatePosition(pos) : void
        + UpdateRotation(rot) : void
        + UpdateScale(scale) : void
        + OnAttached() : void
        + OnDetached() : void
    }

    class CameraComponent {
        + fov : float
        + nearPlane : float
        + farPlane : float
        + width : float
        + height : float
        + perspective : bool
    }

    class MeshComponent {
        + meshAsset : RefPtr<MeshAsset>
    }

    class MaterialComponent {
        + materialAsset : RefPtr<MaterialAsset>
    }

    class LightComponent {
        + direction : fvec3
    }

    class ColorComponent {
        + red : float
        + green : float
        + blue : float
        + alpha : float
    }

    abstract class BaseNode <<ECS::Node>> {
    }

    class MeshNode {
        + transform : ObjectPtr<SceneComponent>
        + mesh : ObjectPtr<MeshComponent>
        + overrideMaterial : ObjectPtr<MaterialComponent>
    }

    class CameraNode {
        + transform : ObjectPtr<SceneComponent>
        + camera : ObjectPtr<CameraComponent>
    }

    class LightNode {
        + transform : ObjectPtr<SceneComponent>
        + light : ObjectPtr<LightComponent>
    }

    abstract class BaseSystem <<ECS::System>> {
    }

    class TypedSystem<T> {
        # UpdateInternal(timeStep, container) : virtual void
    }

    class InputSystem<T> {
        # inputStorage : RefPtr<InputStorage>
    }
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

@enduml
```

---

### 3. Renderer — Rendering Abstraction Layer

References Unreal Engine's `FScene`, `FPrimitiveSceneProxy`, `FMeshBatch`, and `FRenderingCompositePassContext`. World objects are never passed directly to the renderer — they are converted into **Proxy** objects. `RenderScene` reorganizes these Proxies into `MeshBatch` instances to implement instancing.

```plantuml
@startuml Renderer_ClassDiagram

skinparam classAttributeIconSize 0
skinparam BackgroundColor #FAFAFA

package "Renderer" {

    class Renderer {
        - m_refScene : RefPtr<RenderScene>
        - m_refGraph : RefPtr<RenderGraph>
        - m_refCommandList : RefPtr<RenderCommandList>
        - m_renderViews : DynamicArray<RenderView>
        + Init() : bool
        + Execute(cmdList) : void
        + SetView(view) : void
        + PreDraw(cmdList) : void
        + Draw(cmdList) : void
        + PostDraw(cmdList) : void
        + GetScene() : RefPtr<RenderScene>
        + GetGraph() : RefPtr<RenderGraph>
    }

    class RenderScene {
        - m_primitives : HashMap<UUID, RefPtr<PrimitiveProxy>>
        - m_lights : HashMap<UUID, RefPtr<LightProxy>>
        - m_meshBatches : HashMap<MeshBatchKey, RefPtr<MeshBatch>>
        - m_pendingPrimitives : DynamicArray<RefPtr<PrimitiveProxy>>
        + Flush(cmdList) : void
        + UpdateProxy(updateInfo, cmdList) : void
        + AddPrimitive(primitive) : void
        + RemovePrimitive(id, cmdList) : void
        + AddLight(light) : void
        + RemoveLight(id) : void
        + AddBatch(primitive, cmdList) : void
        + UpdateBatch(primitive, cmdList) : void
        + RemoveBatch(primitive, cmdList) : void
        + GetMeshBatch(key) : RefPtr<MeshBatch>
    }

    class RenderGraph <<ECS::Graph<PipeLine>>> {
        - m_addable : HashSet<RefPtr<PipeLine>>
        - m_removable : HashSet<RefPtr<PipeLine>>
        - m_globalResource : RefPtr<GlobalResource>
        - m_drawCommands : DynamicArray<RefPtr<MeshDrawCommand>>
        + Init(cmdList) : bool
        + Execute(cmdList, renderScene, renderView) : void
        + Flush(cmdList) : void
        + Add(pipeline) : void
        + Remove(pipeline) : void
    }

    class GlobalResource {
        + struct CameraData { viewMatrix, projMatrix, cameraPos }
        + struct BufferResource { cameraBuffer }
        + struct TextureResource { position, normal, albedo, depth, stencil }
        + struct ScreenQuadResource { posBuffer, uvBuffer, indexBuffer }
        + Init(cmdList) : bool
        + UpdateCamera(renderView, cmdList) : void
        + GetCameraBuffer() : RefPtr<RHIBuffer>
        + GetScreenQuad() : RefPtr<RHIVertexLayout>
    }

    abstract class PipeLine <<ECS::Object, RenderResource>> {
        - m_pipeLine : RefPtr<RHIPipeLine>
        - m_prepared : bool
        + Execute(commands, globalResource, cmdList) : void
        + Init() : virtual void
        + Prepare() : virtual void
        + Draw(commands, globalResource, cmdList) : virtual void
        + GetShaderState() : virtual eResourceState
    }

    class SimpleColor {
        + Init() : void
        + Prepare() : void
        + Draw(commands, globalResource, cmdList) : void
    }

    class RenderCommandList {
        - m_head : atomic<RenderTask*>
        - m_tail : atomic<RenderTask*>
        - m_allocator[2] : LinearArena
        - m_writeIndex : atomic<size_t>
        - m_readIndex : size_t
        + Enqueue(func) : void
        + ExecuteAll(renderer, cmdList) : void
        + Reset() : void
    }

    class SceneProxy {
        - m_position : fvec3
        - m_rotation : fvec3
        - m_scale : fvec3
        + UpdatePosition(pos) : void
        + UpdateRotation(rot) : void
        + UpdateScale(scale) : void
        + GetTransform() : fmat4
    }

    class PrimitiveProxy {
        - m_refMesh : RefPtr<const MeshAsset>
        - m_refOverrideMaterial : RefPtr<const MaterialAsset>
        + SetMesh(refMesh) : void
        + SetOverrideMaterial(refMaterial) : void
        + GetMesh() : RefPtr<const MeshAsset>
    }

    class LightProxy {
    }

    class MeshBatch {
        - m_transformInfos : HashMap<UUID, TransformInfo>
        - m_transformBuffer : RefPtr<RHIBuffer>
        - m_refDrawCommand : RefPtr<MeshDrawCommand>
        - m_refMesh : RefPtr<const MeshAsset>
        - m_refMaterial : RefPtr<const MaterialAsset>
        - m_sectionIndex : size_t
        + Upload(cmdList) : void
        + Unload(cmdList) : void
        + Sync(cmdList) : void
        + AddTransform(id, transform) : void
        + UpdateTransform(id, transform) : void
        + RemoveTransform(id) : void
        + GetInstanceCount() : size_t
        + GetDrawCommand() : RefPtr<MeshDrawCommand>
    }

    class MeshDrawCommand {
        + vertexLayout : RefPtr<RHIVertexLayout>
        + indexOffset : uint32_t
        + indexCount : uint32_t
        + textureSlots : HashMap<eTextureSlot, RefPtr<RHITexture>>
        + vectorValues : HashMap<eVectorSlot, fvec3>
        + scalarValues : HashMap<eScalarSlot, float>
        + instanceBuffer : RefPtr<RHIBuffer>
        + instanceCount : uint32_t
    }

    class MeshBatchKey {
        + meshId : UUID
        + materialId : UUID
        + meshSection : size_t
        + operator==() : bool
    }

    struct UpdateProxyInfo {
        + id : UUID
        + position : fvec3
        + rotation : fvec3
        + scale : fvec3
    }

    struct RenderView {
        + viewMatrix : fmat4
        + projectionMatrix : fmat4
        + cameraPosition : fvec3
    }
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

@enduml
```

---

### 4. RHI — Render Hardware Interface

References Unreal Engine's `FRHICommandList`, `FRHIResource`, and `FRHICommandListExecutor` patterns. Abstracts the actual GPU API (OpenGL / DirectX / Vulkan), and all GPU commands are encapsulated as **Command Objects** before being executed serially.

```plantuml
@startuml RHI_ClassDiagram

skinparam classAttributeIconSize 0
skinparam BackgroundColor #FAFAFA

package "RHI" {

    abstract class RHISystem {
        + Init(nativeHandle) : virtual bool
        + Clear(state) : virtual void
        + Flush() : virtual void
        + Present() : virtual void
        + Resize(...) : virtual void
        + SetColorState(state) : virtual void
        + SetDepthState(state) : virtual void
        + SetStencilState(state) : virtual void
        + SetBlendState(state) : virtual void
        + SetRasterizerState(state) : virtual void
        + CreateBuffer(desc) : virtual RefPtr<RHIBuffer>
        + CreateVertexLayout(desc) : virtual RefPtr<RHIVertexLayout>
        + CreateTexture(desc) : virtual RefPtr<RHITexture>
        + CreateSampler(desc) : virtual RefPtr<RHISampler>
        + CreateVertexShader(desc) : virtual RefPtr<RHIShader>
        + CreatePixelShader(desc) : virtual RefPtr<RHIShader>
        + CreatePipeLine(desc) : virtual RefPtr<RHIPipeLine>
        + InitializeBuffer(info, buffer) : virtual void
        + UpdateBuffer(info, buffer) : virtual void
        + DrawIndexPrimitive(info) : virtual void
        + DispatchCompute(info) : virtual void
        ... (40+ methods)
    }

    class GLSystem {
        (OpenGL 4.5 implementation)
    }

    class RHIResource {
        - m_state : atomic<eResourceState>
        + GetRawBuffer() : void*
        + SetState(eState) : void
        + GetState() : eResourceState
    }

    class RHIBuffer {
        - m_desc : RHIBufferDesc
        + GetBufferType() : eBufferType
        + GetAccessType() : eDataAccess
        + GetSize() : uint32_t
        + GetStride() : uint32_t
        + GetCount() : uint32_t
    }

    class RHIVertexLayout {
        - m_desc : RHIVertexLayoutDesc
        + GetNumAttributes() : size_t
    }

    class RHITexture {
        - m_desc : RHITextureDesc
        + GetWidth() : uint32_t
        + GetHeight() : uint32_t
        + GetPixelFormat() : ePixelFormat
        + GetTextureType() : eTextureType
    }

    class RHISampler {
        - m_desc : RHISamplerDesc
        + GetMinFilter() : eFilterMode
        + GetMagFilter() : eFilterMode
    }

    class RHIShader {
        - m_desc : RHIShaderDesc
        + GetShaderType() : eShaderType
    }

    class RHIPipeLine {
        - m_desc : RHIPipeLineDesc
        - m_slots : HashMap<string, RHIResourceBinding>
        + GetClearState() : RHIClearState
        + GetDepthState() : RHIDepthState
        + AddSlot(name, binding) : void
        + HasSlot(name) : bool
        + GetBindingSlot(name) : RHIResourceBinding
    }

    class RHICommandList {
        - m_system : RefPtr<RHISystem>
        - m_commands : DynamicArray<RHICommandBase*>
        - m_frame : size_t
        + Enqueue<T>(...) : void
        + ExecuteAll() : void
        + Reset() : void
        + SetFrame(frame) : void
        + CreateBuffer(info) : RefPtr<RHIBuffer>
        + InitializeBuffer(info, buffer) : void
        + UpdateBuffer(info, buffer) : void
        + DrawIndexPrimitive(info) : void
        ... (delegates to command objects)
    }

    class RHICommandBase {
        - m_func : ExecuteFunc
        + Execute(system) : void
    }

    class "RHICommand<T>" as RHICommandT {
        + ExecuteAndDestruct(system, base) : static void
    }

    note right of RHICommandT
      45+ command classes:
      RHICommandClear
      RHICommandFlush
      RHICommandPresent
      RHICommandInitializeBuffer
      RHICommandUpdateBuffer
      RHICommandInitializeTexture
      RHICommandInitializePipeLine
      RHICommandSetPipeLine
      RHICommandDrawIndexPrimitive
      RHICommandDispatchCompute
      ...
    end note

    abstract class RHIExecutor {
        - m_system : RefPtr<RHISystem>
        + Acquire() : virtual RefPtr<RHICommandList>
        + Submit(cmdList) : virtual void
        + Execute() : virtual void
    }

    class RHIFrameExecutor {
        - m_frameCount : size_t
        - m_recordIndex : size_t
        - m_beginIndex : size_t
        - m_endIndex : atomic<size_t>
        - m_listPool : DynamicArray<RefPtr<RHICommandList>>
        + Acquire() : RefPtr<RHICommandList>
        + Submit(cmdList) : void
        + Execute() : void
        + Init(frameCount) : bool
    }

    class RHITaskExecutor {
        (Command list pool for asset/task uploads)
        + Acquire() : RefPtr<RHICommandList>
        + Submit(cmdList) : void
        + Execute() : void
    }
}

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

@enduml
```

---

### 5. Asset — Async Asset Loading System

Asset loading is organized as a 3-stage pipeline: **Parse (file read)** → **Load (GPU format conversion)** → **Upload (GPU upload)**. Each stage runs in a different thread context, ensuring the main loop is never blocked.

```plantuml
@startuml Asset_ClassDiagram

skinparam classAttributeIconSize 0
skinparam BackgroundColor #FAFAFA

package "Asset" {

    class Asset <<ECS::Object>> {
        + id : UUID
        + name : string
        + path : const string
        + extension : const eExtension
        + type : const eAsset
        - m_state : atomic<eAssetState>
        + GetResourceState() : virtual eResourceState
        + SetState(state) : void
        + GetState() : eAssetState
    }

    class MeshAsset {
        + rawBuffers : HashMap<VertexKey, RefPtr<FormattedBuffer>>
        + buffers : HashMap<VertexKey, RefPtr<RHIBuffer>>
        + rawIndex : RefPtr<FormattedBuffer>
        + index : RefPtr<RHIBuffer>
        + sections : DynamicArray<MeshSection>
        + materials : HashMap<string, RefPtr<MaterialAsset>>
        + GetResourceState() : eResourceState
    }

    class MaterialAsset {
        + textures : HashMap<eTextureSlot, RefPtr<TextureAsset>>
        + vectorValues : HashMap<eVectorSlot, fvec3>
        + scalarValues : HashMap<eScalarSlot, float>
        + GetResourceState() : eResourceState
    }

    class TextureAsset {
        + rawBuffer : RefPtr<FormattedBuffer>
        + texture : RefPtr<RHITexture>
        + width : uint32_t
        + height : uint32_t
        + pixelFormat : ePixelFormat
        + GetResourceState() : eResourceState
    }

    class ShaderAsset {
        + rawBuffer : RefPtr<RawBuffer>
        + shader : RefPtr<RHIShader>
        - m_shaderType : eShaderType
        + GetResourceState() : eResourceState
        + SetShaderType(type) : void
    }

    class AssetSystem {
        + Load(path) : static RefPtr<Asset>
        + Unload(path) : static void
        + Shutdown() : static void
        + GetTask() : static TaskQueue
        + AddTask(asset) : static void
        + Release(cmdList) : static void
        + GetParser(path) : static RefPtr<AssetParser>
    }

    class AssetFactory {
        + Create(path) : RefPtr<Asset>
        + Create(path, extension) : RefPtr<Asset>
        + GetExtension(path) : eExtension
    }

    abstract class AssetParser {
        + Parse(asset) : virtual void
        + Load(asset, cmdList) : virtual void
        + Unload(asset, cmdList) : virtual void
    }

    class OBJParser {
        + Parse(asset) : void
        + Load(asset, cmdList) : void
    }

    class MTLParser {
        + Parse(asset) : void
        + Load(asset, cmdList) : void
    }

    class GLSLParser {
        + Parse(asset) : void
        + Load(asset, cmdList) : void
    }

    class StbImageParser {
        + Parse(asset) : void
        + Load(asset, cmdList) : void
    }

    struct MeshSection {
        + indexOffset : uint32_t
        + indexCount : uint32_t
        + minVertexIndex : uint32_t
        + maxVertexIndex : uint32_t
        + materialName : string
        + name : string
    }
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

@enduml
```

---

## Threading and Data Flow

### Thread Overview

The engine consists of 4 independent worker threads and 1 task thread pool.

```plantuml
@startuml Thread_Overview

skinparam BackgroundColor #FAFAFA

rectangle "Main Thread" as main {
    [Window Event Loop\n+ Input Polling]
}

rectangle "WorldWorker\n(Logic Thread)" as world {
    [ECS Update\nCommander\nFill RenderCommandList]
}

rectangle "RenderWorker\n(Render Thread)" as render {
    [RenderScene Flush\nMeshBatch Sync\nRenderGraph Execute\nFill RHICommandList]
}

rectangle "RHIWorker\n(GPU Submit Thread)" as rhi {
    [RHIFrameExecutor Execute\nRHITaskExecutor Execute\nOpenGL / GPU API calls]
}

rectangle "AssetWorker\n(Asset Thread)" as asset {
    [Consume AssetSystem Queue\nDispatch to Parsers]
}

rectangle "TaskWorker Pool\n(N threads)" as task {
    [OBJ / PNG / GLSL\nFile parsing (CPU-bound)]
}

main --> world : InputStorage
world --> render : FrameGate (updateGate)\n+ RenderCommandList
render --> rhi : FrameGate (renderGate)\n+ RHICommandList (Frame)
asset --> rhi : RHICommandList (Task)\n(GPU upload)
asset --> task : Dispatch parse tasks

@enduml
```

---

### Frame Update Sequence — World → Renderer → RHI

`FrameGate` implements producer-consumer synchronization between World and Renderer, and between Renderer and RHI.

```plantuml
@startuml Frame_Sequence

skinparam BackgroundColor #FAFAFA
skinparam SequenceArrowThickness 1.5

participant "WorldWorker\n(Logic Thread)" as WW
participant "FrameGate\n(updateGate)" as FG1
participant "RenderCommandList" as RCL
participant "RenderWorker\n(Render Thread)" as RW
participant "FrameGate\n(renderGate)" as FG2
participant "RHICommandList\n(Frame)" as RHICL
participant "RHIWorker\n(GPU Submit)" as RHIW
participant "GPU (OpenGL)" as GPU

== Frame N Start ==

WW -> WW : ECS Update\n(Systems, Components)
WW -> RCL : Commander.AddPrimitive()\nCommander.Update()\nCommander.SetView()
note right of RCL : RenderTask lambdas stored\nin LinearArena\n(lock-free queue)
WW -> FG1 : Submit()\n(m_frameDiff++)
note right of FG1 : condition_variable\nwakes RenderWorker

== Render Thread Processing ==

FG1 -> RW : Acquire() done\n(m_frameDiff--)
RW -> RHICL : Executor.Acquire()\n(acquire next frame command list)
RW -> RCL : ExecuteAll(renderer, cmdList)\n(execute all RenderTasks)
note right of RCL
  Inside each task:
  RenderScene.AddBatch()
  RenderScene.UpdateBatch()
  RenderScene.Flush()
  GlobalResource.UpdateCamera()
end note
RW -> RHICL : RenderGraph.Execute()\n(PipeLine.Draw() -> generate draw commands)
RW -> RHICL : Enqueue<RHICommandDraw>()
RW -> RHICL : Executor.Submit(cmdList)
RW -> FG2 : Submit()\n(m_frameDiff++)

== RHI Thread Processing ==

FG2 -> RHIW : Acquire() done
RHIW -> RHICL : FrameExecutor.Execute()\n(consume command lists)
RHICL -> GPU : RHICommand::Execute(RHISystem)\n(OpenGL API calls)
GPU -> GPU : DrawIndexPrimitive\nPresent
RHIW -> RHIW : TaskExecutor.Execute()\n(process asset upload commands)

@enduml
```

---

### Asset Loading Sequence — Async 3-Stage Pipeline

```plantuml
@startuml Asset_Sequence

skinparam BackgroundColor #FAFAFA

participant "Game Code\n(Main Thread)" as GC
participant "AssetSystem" as AS
participant "AssetWorker\n(Asset Thread)" as AW
participant "TaskWorker Pool\n(Parse Threads)" as TW
participant "RHITaskExecutor\n(Task CmdList)" as RTE
participant "RHIWorker\n(GPU Submit)" as RHIW
participant "GPU" as GPU

== Asset Load Request ==

GC -> AS : AssetSystem::Load("mesh.obj")
AS -> AS : AssetFactory::Create(path)\n-> MeshAsset created\n(state = eLoaded)
AS -> AS : Enqueue(asset) to internal queue

== Parse Stage (CPU-bound) ==

AW -> AS : GetTask() -> dequeue MeshAsset
AW -> TW : TaskWorker::Set(ParseTask)\n(condition_variable notify)
TW -> TW : OBJParser::Parse(asset)\nread file -> fill rawBuffers\n(state = eParsed)
TW --> AW : completion signal

== Load Stage (GPU format conversion) ==

AW -> RTE : Executor.Acquire()\n-> acquire RHICommandList
AW -> RTE : OBJParser::Load(asset, cmdList)\n-> Enqueue<RHICommandInitializeBuffer>()\n-> Enqueue<RHICommandInitializeTexture>()
AW -> RTE : Executor.Submit(cmdList)

== GPU Upload Stage ==

RHIW -> RTE : TaskExecutor.Execute()
RTE -> GPU : RHICommand::Execute(RHISystem)\n-> glBufferData / glTexImage2D
GPU --> RHIW : done
note right of RHIW
  asset.state = eReady
  MeshBatch can now
  reference RHIBuffer
end note

@enduml
```

---

### Synchronization Mechanism Detail

```plantuml
@startuml Sync_Detail

skinparam BackgroundColor #FAFAFA

package "FrameGate Internal Structure" {
    class FrameGate {
        - m_cvProducer : condition_variable
        - m_cvConsumer : condition_variable
        - m_mutex : mutex
        - m_frameLimit : const size_t
        - m_frameDiff : atomic<int32_t>
        - m_notified : atomic<bool>
        ..
        Submit() {
          lock(m_mutex)
          m_frameDiff++
          m_cvConsumer.notify_one()
        }
        ..
        Acquire() {
          lock(m_mutex)
          m_cvConsumer.wait(
            lock, [&]{ return m_frameDiff > 0 }
          )
          m_frameDiff--
        }
    }
}

package "RHIFrameExecutor Internal Structure (Triple Buffering)" {
    class RHIFrameExecutor {
        - m_listPool[3] : RHICommandList[]
        - m_recordIndex : size_t   <- written by RenderWorker
        - m_beginIndex : size_t    <- read start index for RHIWorker
        - m_endIndex : atomic      <- cross-thread: Submit writes, Execute reads
        ..
        Acquire() -> returns m_listPool[m_recordIndex]
        Submit()  -> advances m_recordIndex, increments m_endIndex
        Execute() -> processes range m_beginIndex ~ m_endIndex
    }
}

package "RenderCommandList Internal Structure (Lock-free Queue)" {
    class RenderCommandList {
        - m_head : atomic<RenderTask*>
        - m_tail : atomic<RenderTask*>
        - m_allocator[2] : LinearArena  <- ping-pong buffers
        - m_writeIndex : atomic<size_t>
        ..
        Enqueue() {
          task = m_allocator[writeIdx].Alloc()
          CAS(m_tail, task)  <- lock-free insert
        }
        ExecuteAll() {
          m_writeIndex ^= 1  <- swap buffers
          while(head) head->Execute(); head = head->next
        }
    }
}

@enduml
```

---

### Per-Worker Activity Diagrams

Control flow for each Worker during one frame or one task.

#### WorldWorker (Logic Thread)

```plantuml
@startuml WorldWorker_Activity

skinparam BackgroundColor #FAFAFA
title WorldWorker (Logic Thread)

start

:onStart()\nWorldContext.Init()\nConnect InputStorage;

while (m_isRunning?) is (true)
  :InputStorage.SwapInput()\nAtomic swap: CurrData -> PrevData;
  :WorldContext.Prepare()\nRenderCommandList.Reset();
  :WorldContext.Update(timeStep)\nExecute ECS Systems in order\n(CameraSystem / MoveSystem / ...);
  note right
    During System execution, Commander
    enqueues Scene changes into
    RenderCommandList as RenderTask lambdas
  end note
  :updateGate.Submit()\nm_frameDiff++ -> wake RenderWorker;
endwhile (false)

:onDestroy();
stop

@enduml
```

#### RenderWorker (Render Thread)

```plantuml
@startuml RenderWorker_Activity

skinparam BackgroundColor #FAFAFA
title RenderWorker (Render Thread)

start

:onStart();

while (m_isRunning?) is (true)
  :updateGate.Acquire()\nWait for WorldWorker Submit\n(until m_frameDiff > 0);
  :RHIFrameExecutor.Acquire()\nAcquire RHICommandList from triple buffer\nAdvance m_recordIndex;
  :RenderCommandList.ExecuteAll(renderer, cmdList)\nExecute all RenderTasks\n-> RenderScene Flush / MeshBatch Sync;
  :GlobalResource.UpdateCamera(renderView, cmdList)\nUpload View & Proj matrices to UBO;
  :RenderGraph.Execute(cmdList, renderScene, renderView)\nPipeLine.Draw()\n-> Enqueue RHICommandDrawIndexPrimitive;
  :RHIFrameExecutor.Submit(cmdList)\nAtomic increment m_endIndex\n-> RHIWorker can now read;
  :renderGate.Submit()\nm_frameDiff++ -> wake RHIWorker;
endwhile (false)

:onDestroy();
stop

@enduml
```

#### RHIWorker (GPU Submit Thread)

```plantuml
@startuml RHIWorker_Activity

skinparam BackgroundColor #FAFAFA
title RHIWorker (GPU Submit Thread)

start

:onStart()\nRHISystem.Init()\nInitialize WGLContext (create OpenGL context);

while (m_isRunning?) is (true)
  :renderGate.Acquire()\nWait for RenderWorker Submit;
  :RHIFrameExecutor.Execute()\nConsume range m_beginIndex ~ m_endIndex sequentially;
  note right
    For each RHICommandList.ExecuteAll():
    RHICommandBase::Execute(RHISystem)
    -> direct OpenGL API calls
    After loop: RHISystem.Present()
  end note
  :RHITaskExecutor.Execute()\nProcess asset upload commands\nglBufferData / glTexImage2D;
endwhile (false)

:onDestroy();
stop

@enduml
```

#### AssetWorker (Asset Thread)

```plantuml
@startuml AssetWorker_Activity

skinparam BackgroundColor #FAFAFA
title AssetWorker (Asset Thread)

start

:onStart()\nInitialize TaskWorker pool (N threads);

while (m_isRunning?) is (true)
  :AssetSystem.GetTask()\nCheck for pending assets;
  if (pending asset?) then (yes)
    fork
      :TaskWorker A\nOBJParser.Parse(asset)\nFill rawBuffers\nstate = eParsed;
    fork again
      :TaskWorker B\nStbImageParser.Parse(asset)\nFill rawBuffer\nstate = eParsed;
    end merge
    :RHITaskExecutor.Acquire()\nAcquire RHICommandList;
    :Parser.Load(asset, cmdList)\nGPU format conversion\nEnqueue RHICommandInitializeBuffer\nEnqueue RHICommandInitializeTexture;
    :RHITaskExecutor.Submit(cmdList)\nstate = eReady;
    note right
      RHIWorker later calls TaskExecutor.Execute()
      to perform actual GPU upload
    end note
  else (no)
    :yield / idle;
  endif
endwhile (false)

:onDestroy();
stop

@enduml
```

---

## World → Renderer Data Flow Detail

```plantuml
@startuml DataFlow_Detail

skinparam BackgroundColor #FAFAFA

rectangle "World Layer" {
    [Entity] --> [SceneComponent\n(position/rotation/scale)]
    [Entity] --> [MeshComponent\n(MeshAsset ref)]
    [Entity] --> [MaterialComponent\n(MaterialAsset ref)]
    [SceneComponent] --> [Scene]
    [Scene] --> [Commander]
}

rectangle "Bridge Layer" {
    [Commander] --> [RenderCommandList\n(RenderTask lambda queue)]
    note bottom of [RenderCommandList]
      WorldWorker writes (Producer)
      RenderWorker reads (Consumer)
      Zero-alloc via LinearArena ping-pong
    end note
}

rectangle "Renderer Layer" {
    [RenderCommandList] --> [RenderScene]
    [RenderScene] --> [PrimitiveProxy\n(SceneProxy + Mesh/Material)]
    [PrimitiveProxy] --> [MeshBatch\n(grouped by Mesh+Material+Section key)]
    [MeshBatch] --> [MeshDrawCommand\n(VBO, IBO, instance buffer, texture slots)]
    [MeshDrawCommand] --> [RenderGraph]
    [RenderGraph] --> [PipeLine\n(SimpleColor / Deferred planned)]
}

rectangle "RHI Layer" {
    [PipeLine] --> [RHICommandList\n(GPU command object queue)]
    [RHICommandList] --> [RHISystem\n(GLSystem)]
    [RHISystem] --> [GPU\n(OpenGL 4.5)]
}

@enduml
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
git clone --recursive https://github.com/YourUsername/GameEngine.git
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
