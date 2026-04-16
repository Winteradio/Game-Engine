# 커밋 히스토리 요약 (Commit History Summary)

각 커밋의 주된 변경 사항을 날짜순으로 정리한 문서입니다.

---

## 2023-03-23 — `e211785`
### [Title] Complete Load and Save Project file
- 프로젝트의 최초 커밋
- CMake 빌드 시스템 구성 (CMakeLists.txt, Dependency.cmake)
- DirectX 11 기반의 엔진 코어(CoreEngine) 설계 및 구현
- 프로젝트 파일 로드/저장 기능 구현 (FileDialog, ProjectManager)
- Layer 시스템(GameLayer, FileBrowserGuiLayer, PropertyBrowserGuiLayer) 추가
- ImGui UI 연동 (ImGuiAPI)
- DirectX Input / Shader / UUID 유틸리티 추가

---

## 2023-03-24 — `d26fd6fa`
### [Title] Envolve Old version Engine
- 구버전 엔진 코드 전체를 `GameEngine/` 서브폴더로 통합
- ImGui 소스, 폰트 파일(Nanum, font.ttf), UML 다이어그램 추가
- `.gitmodules`, `Sublime Text` 프로젝트 설정 파일 추가

---

## 2023-03-28 — `0e137a7b`
### [Title] Input Shader Interface and Make source file vertex and pixel shader
- `GameEngine/` 서브폴더 제거 후 소스 구조 정리
- Vertex/Pixel Shader 인터페이스 파일 추가 (`.hlsl` 연동 구조 도입)
- `src/` 하위 구조 재편 및 `.gitignore` 업데이트

---

## 2023-03-29 — `c34ad859`
### [Title] 2023-03-29
- `PropertyBrowserGuiLayer` 소규모 버그 수정
- `Log.cpp` 출력 로직 수정

---

## 2023-04-01 — `ba10d84c`
### [Title] Idea for Entity Component System in DirectX11 C++
- ECS(Entity Component System) 설계 아이디어 반영
- Manager 패턴 도입: CameraManager, LightManager, MeshManager, ObjectManager, SceneManager, ShaderManager, TextureManager
- `UML/NewEngine.xmi` UML 다이어그램 추가
- `GameLayer` 및 `LayerManager` 업데이트

---

## 2023-04-21 — `a09c3be7`
### [Title] ECS Refactoring
- 엔진 코어를 `Engine/Core`로 재구성 (기존 CoreEngine → Core)
- ECS 구조 도입: `Entity`, `ComponentManager`, `NodeManager` 클래스 추가
- `Scene` → `SceneManager` 분리
- DirectX API 파일들을 `src/API/DirectX/`로 이동

---

## 2023-04-28 — `74f539e3`
### [Title] Complete NodeManager
- `NodeManager` 완성
- 소스 디렉터리를 `src/API/`로 재편 (DirectX, ImGui, UUID, Window)
- `.vscode` 설정 파일(launch.json, c_cpp_properties.json) 추가
- CMakeLists.txt 대폭 정리 및 업데이트

---

## 2023-05-03 — `fad3db2f`
### [Title] 2023-05-03
- `SystemManager` 업데이트 (시스템 실행 순서 관리 개선)
- CMakeLists.txt 수정 및 VS 프로젝트 설정 추가
- `Core.cpp` 초기화 로직 수정

---

## 2023-05-10 — `2a7a7b66`
### [Title] Making UI System for ECS
- ECS용 UI 시스템 개발 시작
- 에셋 폴더(`asset/font/`, `asset/image/`) 및 폰트·이미지 파일 추가
- CMakeLists.txt 개편, `CppProperties.json` 제거
- `.vscode/settings.json` 추가

---

## 2023-05-13 — `ab038021`
### [Title] Component Manager's reallocing problem
- `ComponentManager`의 벡터 재할당 시 포인터 무효화 문제 파악 및 수정
- 기존 `src/Engine` 코드를 `OldVersionEngine2/`로 이동하여 레거시 보존
- `.vscode/settings.json`에 IntelliSense 설정 확장

---

## 2023-05-14 — `82926fa0`
### [Title] Before, Add EntityManager, save this point.
- `EntityManager` 추가 (엔티티 생성·제거·조회를 UUID 기반으로 관리)
- `Scene` 역할 분리: Entity/Node UUID 등록 및 System 스케줄링을 Scene이 담당
- `Component`, `Entity`, `Node` 클래스 리팩터링
- ImGui UI 핸들러(MainUIHandler, PropertyUIHandler) 기능 확장

---

## 2023-05-14 — `96ac481a`
### [Title] 70% New Engine
- 신규 엔진 진행률 약 70% 달성
- `FileBrowserUIHandler`, `FileHandler` 추가 (파일 탐색 UI 구현)
- `AssetHandler` 기능 확장 (에셋 로드/저장)
- `Scene` 전면 개편, YAML 예제 파일(`Example1.yaml`) 추가

---

## 2023-05-16 — `9dd0c04d`
### [Title] FileSystem Error
- Visual Studio 2019 컴파일러의 C++17 `filesystem` 미지원 문제 발견 및 수정
- CMake 제너레이터를 Visual Studio 17 2022로 변경하여 해결
- UML 다이어그램(`NewEngine.xmi`) 대폭 업데이트
- `FileBrowserUIHandler`, `FileHandler` 버그 수정

---

## 2023-05-17 — `d3165c7b`
### [Title] ...
- `AssetHandler` 직렬화/역직렬화 로직 개선
- `FileBrowserUIHandler` UI 흐름 리팩터링
- `MenuBarUIHandler` 메뉴 항목 추가
- YAML 예제(`Example1.yaml`) 업데이트

---

## 2023-05-19 — `8b3bb272`
### [Title] New Idea for using memory pool
- `MemoryManager` 추가 (메모리 풀 아이디어 도입)
- `README.txt` → `README.md`로 전환 (마크다운 형식으로 문서화)
- `SystemManager`에 시스템 등록·실행 구조 개선
- `AssetHandler` 기능 확장

---

## 2025-12-18 — `0e6eb58e`
### [Title] Set the Game Engine Development environment
- **엔진 전면 재설계**: 기존 DirectX 11 기반 코드 전체 제거
- **OpenGL 기반** 신규 엔진 환경 구축
- `git submodule`로 모듈화된 외부 라이브러리 도입:
  - `wtr` 시리즈: Log, Container, Reflection, Memory, ECS
  - GL: `glad`, `glm`
  - WGL: Win32 OpenGL 컨텍스트 관리
- CMakeLists.txt 전면 재작성

---

## 2025-12-18 — `4bf0f715` / `d27aa678` (Merge PR #2) / `a11097e2` / `f96c9521`
### [Title] Add the README.md / Update README.md
- 신규 엔진에 맞는 `README.md` 초안 작성
- README 내용 반복 수정 및 개선 (PR #2 병합)

---

## 2025-12-31 — `1c4c68aa` / `753f603f` (Merge PR #3)
### [Title] Add the basic file for the renderer and RHI
- **프레임워크 기초 구현**:
  - `Engine`, `CommandList`, `InputStorage`, `InputTypes`, `Worker`
- **Win32 플랫폼 레이어** 추가: `Win32Window`, `Win32InputHandler`
- **RHI (Render Hardware Interface)** 기초 파일 추가:
  - `WGLContext`, `WGLDevice`, `GLResources`
- `Client/src/main.cpp` 엔트리 포인트 추가 (PR #3 병합)

---

## 2025-12-31 — `49b40b6a`
### [Title] Edit the InputStorage for the parallel input handling
- `InputStorage`를 병렬 입력 처리에 맞게 리팩터링
- `RenderCommandList`, `RenderContext`, `RenderWorker` 추가 (렌더 스레드 기반 구조)
- `WorldWorker` 추가 (월드 로직 스레드 분리)
- `CommandList` 및 `Worker` 구조 개선

---

## 2026-01-14 — `ca90c14a`
### [Title] Simple Rectangle
- `DemoApplication` 클래스 추가 (클라이언트 데모 분리)
- `WGLContext` 구현 완성 (OpenGL 컨텍스트 초기화 전체 구현, 159줄 추가)
- `GLSystem` 추가 (OpenGL 상태 관리)
- `RHICommandList`, `RHICommands` 기초 구현
- `FrameContext` 도입 (기존 `RenderContext` 대체)
- 화면에 단순 사각형(배경색) 렌더링 가능

---

## 2026-01-27 — `736602c9`
### [Title] Base Architecture for the RHI and Proxy
- **Proxy 시스템 설계**: `RenderProxy`, `PrimitiveProxy`, `MeshProxy`, `MaterialProxy`
- **씬 뷰 구조**: `FrameView`, `SceneView` 추가
- `PipeLineGraph` 구조 도입 (렌더 파이프라인 그래프)
- `ProxyList` 컨테이너 추가
- `FrameContext` 개선 및 `DemoApplication` 업데이트

---

## 2026-02-01 — `492ce4cf`
### [Title] Add the asset module
- **에셋 모듈** 추가: `AssetLoader`, `AssetManager`, `AssetParser`, `AssetWorker`, `AssetResult`
- `GLSystem` 전면 구현 (버퍼·셰이더·텍스처 생성·바인딩 등 709줄)
- `RHISystem`, `RHIResources` 확장 (버퍼·텍스처·파이프라인 리소스 정의)
- `RHICommandExecutor`, `RHIWorker` 추가 (RHI 커맨드 비동기 실행)
- `PipeLineGraph` 서브디렉터리로 재구성

---

## 2026-02-21 — `94983503`
### [Title] Edit for the test demo
- `TaskWorker` 추가 (태스크 기반 비동기 작업 처리)
- `GLResources` 확장 (GL 리소스 타입 정의 110줄 추가)
- `RHICommandList`, `RHICommands` 대폭 확장 (각각 175줄, 323줄 추가)
- `RHICommandExecutor` 업데이트 (커맨드 실행 루프 구현)
- `AssetWorker`, `AssetLoader` 개선

---

## 2026-02-21 — `2a7a42af`
### [Title] Test for the simple background color in the multi-thread
- **멀티스레드 렌더링 버그 수정**: WGL 컨텍스트 소유권(Ownership)을 RHI 스레드에 올바르게 이전
- `RHICommandExecutor`에 `ExecuteAndDestruct` 패턴 도입 (커맨드 실행 후 즉시 해제)
- `RenderGraph`, `RenderWorker` 개선 (렌더 스레드 흐름 정비)
- 멀티스레드 환경에서 단색 배경 렌더링 성공

---

## 2026-02-22 — `cd4221a8`
### [Title] Add the Frame and Task executor
- `RHICommandExecutor` 제거 후 **두 가지 실행기로 분리**:
  - `RHIFrameExecutor`: 프레임 카운트 기반 블로킹 SPSC 실행기 (렌더 스레드 동기화)
  - `RHITaskExecutor`: 태스크 단위 비동기 실행기
- `RHIExecutor` 인터페이스 추가
- `RHIWorker`, `RenderWorker` 업데이트 (새 실행기 적용)
- `FrameContext`, `FrameView` 개선

---

## 2026-02-22 — `52706704`
### [Title] Update the submodules
- `external/Container`, `external/ECS`, `external/Log`, `external/Memory` 서브모듈을 최신 버전으로 업데이트

---

## 2026-03-12 — `81520091` (Merge PR #5)
### Merge pull request #5 from Winteradio/update/base-cube
- `update/base-cube` 브랜치 작업 내용 main에 병합

---

## 2026-04-03 — `d34b1dd4`
### Initial plan
- Copilot SWE 에이전트에 의한 초기 계획 커밋 (이 히스토리 파일 생성 작업의 시작)

---

*마지막 업데이트: 2026-04-03*
