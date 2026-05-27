# Asset Pipeline Design

Asset loading is organized as a **3-stage pipeline**: Parse (file read) → Load (GPU format conversion) → Upload (GPU upload).  
Each stage runs in a different thread context, ensuring the main loop is never blocked.

---

## Class Diagram

```mermaid
classDiagram
    class Asset {
        + UUID id
        + string name / path
        + eExtension extension
        + eAsset type
        - atomic~eAssetState~ m_state
        + GetResourceState() eResourceState
    }

    class MeshAsset {
        + HashMap~VertexKey_RHIBuffer~ buffers
        + RefPtr~RHIBuffer~ index
        + DynamicArray~MeshSection~ sections
        + HashMap~string_MaterialAsset~ materials
    }

    class MaterialAsset {
        + HashMap~eTextureSlot_TextureAsset~ textures
        + HashMap~eVectorSlot_fvec3~ vectorValues
        + HashMap~eScalarSlot_float~ scalarValues
        + eShadingModel shadingModel
        + bool isPBR / isDoubleSided
    }

    class TextureAsset {
        + RefPtr~RHITexture~ texture
        + uint32_t width / height / mipLevels
        + ePixelFormat pixelFormat
        + bool isSRGB / isCubemap
    }

    class ShaderAsset {
        + RefPtr~RHIShader~ shader
        - eShaderType m_shaderType
    }

    class AssetSystem {
        <<static singleton>>
        + Create(name, type) static RefPtr~Asset~
        + Load(path) static RefPtr~Asset~
        + Update / Unload / Shutdown() static void
    }

    class AssetParser {
        <<abstract>>
        + Parse(asset) void
        + Load(asset, cmdList) void
        + Unload(asset, cmdList) void
    }

    class MeshSection {
        <<struct>>
        + uint32_t indexOffset
        + uint32_t indexCount
        + string materialName
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

    AssetSystem ..> AssetParser : uses
```

---

## State Machine

```
eLoaded → eParsed → eReady
  ↑           ↑         ↑
AssetSystem  TaskWorker  RHIWorker
Create()     Parse()     GPU upload
```

`MeshBatch` only references `RHIBuffer` once asset state is `eReady`.

---

## Technical Challenges

### Async 3-Stage Asset Pipeline
- **Problem**: Loading large OBJ/PNG blocks the main loop; GPU uploads must run exclusively in RHI thread context
- **Solution**:
  - **Parse**: `TaskWorker` pool (parallel CPU) — OBJParser reads file, fills `rawBuffers`
  - **Load**: `AssetWorker` — GPU format conversion, enqueues `RHICommandInitializeBuffer`
  - **Upload**: `RHITaskExecutor` — `RHIWorker` executes `glBufferData / glTexImage2D`
  - `atomic<eAssetState>` tracks transitions safely across threads
