# Threading & Sequences

## Thread Overview

The engine runs 4 independent worker threads plus a task thread pool.

```mermaid
flowchart TD
    main["Main Thread<br>Window Event Loop<br>+ Input Polling"]
    world["WorldWorker<br>(Logic Thread)<br>ECS Update · Commander<br>Fill RenderCommandList"]
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

## Frame Update Sequence — World → Renderer → RHI

`FrameGate` implements producer-consumer synchronization between World↔Renderer and Renderer↔RHI.

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
    WW->>FG1: Submit() (m_frameDiff++)

    FG1->>RW: Acquire() done (m_frameDiff--)
    RW->>RHICL: Executor.Acquire()
    RW->>RCL: ExecuteAll(renderer, cmdList)
    RW->>RHICL: RenderGraph.Execute()
    RW->>RHICL: Executor.Submit(cmdList)
    RW->>FG2: Submit() (m_frameDiff++)

    FG2->>RHIW: Acquire() done
    RHIW->>RHICL: FrameExecutor.Execute()
    RHICL->>GPU: RHICommand::Execute(RHISystem)
    GPU->>GPU: DrawIndexPrimitive · Present
    RHIW->>RHIW: TaskExecutor.Execute()
```

---

## Asset Loading Sequence — Async 3-Stage Pipeline

```mermaid
sequenceDiagram
    participant GC as Game Code
    participant AS as AssetSystem
    participant AW as AssetWorker
    participant TW as TaskWorker Pool
    participant RTE as RHITaskExecutor
    participant RHIW as RHIWorker
    participant GPU as GPU

    GC->>AS: AssetSystem::Load("mesh.obj")
    AS->>AS: AssetFactory::Create(path)<br>MeshAsset created (state = eLoaded)

    AW->>AS: GetTask() -> dequeue MeshAsset
    AW->>TW: TaskWorker::Set(ParseTask)
    TW->>TW: OBJParser::Parse(asset)<br>fill rawBuffers (state = eParsed)

    AW->>RTE: Executor.Acquire()
    AW->>RTE: OBJParser::Load(asset, cmdList)<br>Enqueue RHICommandInitializeBuffer
    AW->>RTE: Executor.Submit(cmdList)

    RHIW->>RTE: TaskExecutor.Execute()
    RTE->>GPU: glBufferData / glTexImage2D
    Note right of RHIW: asset.state = eReady
```

---

## Synchronization Mechanism Detail

```mermaid
classDiagram
    class FrameGate {
        - condition_variable m_cvProducer
        - condition_variable m_cvConsumer
        - mutex m_mutex
        - const size_t m_frameLimit
        - atomic~int32_t~ m_frameDiff
        + Submit()
        + Acquire()
    }
    note for FrameGate "Submit() {\n  lock(m_mutex)\n  m_frameDiff++\n  m_cvConsumer.notify_one()\n}\n\nAcquire() {\n  lock(m_mutex)\n  m_cvConsumer.wait(\n    [&]{ return m_frameDiff > 0 }\n  )\n  m_frameDiff--\n}"

    class RHIFrameExecutor {
        - RHICommandList[] m_listPool
        - size_t m_recordIndex
        - size_t m_beginIndex
        - atomic m_endIndex
        + Acquire()
        + Submit()
        + Execute()
    }
    note for RHIFrameExecutor "Acquire() -> m_listPool[m_recordIndex]\nSubmit() -> advance m_recordIndex, m_endIndex++\nExecute() -> process m_beginIndex ~ m_endIndex\n\nAll indices init to 0 to avoid phantom-slot data race"

    class RenderCommandList {
        - atomic~RenderTask*~ m_head / m_tail
        - LinearArena m_allocator[2]
        - atomic~size_t~ m_writeIndex
        + Enqueue()
        + ExecuteAll()
    }
    note for RenderCommandList "Ping-pong LinearArena:\nEnqueue: writes to allocator[writeIndex]\nExecuteAll: swaps writeIndex, drains other buffer\n-> Zero-alloc, lock-free producer-consumer"
```

---

## Per-Worker Activity Diagrams

### WorldWorker (Logic Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart(): WorldContext.Init()"]
    Init --> Loop{"m_isRunning?"}
    Loop -->|true| Swap["InputStorage.SwapInput()"]
    Swap --> Prepare["WorldContext.Prepare(): RenderCommandList.Reset()"]
    Prepare --> Update["WorldContext.Update(timeStep): ECS Systems"]
    Update --> Submit["updateGate.Submit()"]
    Submit --> Loop
    Loop -->|false| Stop([Stop])
```

### RenderWorker (Render Thread)

```mermaid
flowchart TD
    Start([Start]) --> Loop{"m_isRunning?"}
    Loop -->|true| Acq1["updateGate.Acquire()"]
    Acq1 --> Acq2["RHIFrameExecutor.Acquire()"]
    Acq2 --> Exec["RenderCommandList.ExecuteAll()"]
    Exec --> Cam["GlobalResource.UpdateCamera()"]
    Cam --> Graph["RenderGraph.Execute()"]
    Graph --> SubRHI["RHIFrameExecutor.Submit()"]
    SubRHI --> SubGate["renderGate.Submit()"]
    SubGate --> Loop
    Loop -->|false| Stop([Stop])
```

### RHIWorker (GPU Submit Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart(): RHISystem.Init() (WGLContext)"]
    Init --> Loop{"m_isRunning?"}
    Loop -->|true| Acq["renderGate.Acquire()"]
    Acq --> ExecFrame["RHIFrameExecutor.Execute(): OpenGL API calls + Present"]
    ExecFrame --> ExecTask["RHITaskExecutor.Execute(): asset uploads"]
    ExecTask --> Loop
    Loop -->|false| Stop([Stop])
```

### AssetWorker (Asset Thread)

```mermaid
flowchart TD
    Start([Start]) --> Init["onStart(): TaskWorker pool init"]
    Init --> Loop{"m_isRunning?"}
    Loop -->|true| Check["AssetSystem.GetTask()"]
    Check --> Cond{"pending?"}
    Cond -->|yes| Parse["TaskWorker: Parser.Parse(asset)"]
    Parse --> Load["Parser.Load(asset, cmdList): GPU format conversion"]
    Load --> Sub["RHITaskExecutor.Submit(cmdList)"]
    Sub --> Loop
    Cond -->|no| Idle["yield"] --> Loop
    Loop -->|false| Stop([Stop])
```
