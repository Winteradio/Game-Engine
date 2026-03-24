# ------ Set the basic options for the GameEngine project ------- #
message(STATUS "# Set the basic options for the GameEngine project")

cmake_minimum_required(VERSION 3.20)
project(GameEngine LANGUAGES C CXX)

# ------ Create the executable program ------ #
if (WIN32)
    message(STATUS "- GameEngine Platform : Windows")
    add_executable(${PROJECT_NAME} WIN32)
else ()
    message(FATAL_ERROR "- GameEngine Platform : Others (Not supported yet)")
endif()

# ------ Set the project options ------ #
include(${CMAKE_SOURCE_DIR}/external/CMake/Project.cmake)
project_options()

# ------ Add the sub modules ------ #
message(STATUS "- Add the Sub Project")

set(LOG_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Log Log)

set(CONTAINER_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Container Container)

set(REFLECTION_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Reflection Reflection)

set(MEMORY_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Memory Memory)

set(ECS_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/ECS ECS)

set(GLM_BUILD_TESTS OFF CACHE BOOL "Build GLM tests" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm glm)

set(YAML_BUILD_SHARED_LIBS true)
set(YAML_CPP_BUILD_TOOLS OFF)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE) # CTest 타겟 생성 방지
add_subdirectory(${CMAKE_SOURCE_DIR}/external/yaml-cpp yaml-cpp)

# ------ Set the properties the executable program ------ #
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

set(IMGUI_FILES
	${EXTERNAL_DIR}/imgui/imgui.cpp
	${EXTERNAL_DIR}/imgui/imgui_demo.cpp
	${EXTERNAL_DIR}/imgui/imgui_draw.cpp
	${EXTERNAL_DIR}/imgui/imgui_tables.cpp
	${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
)

set(GLAD_FILES
	${EXTERNAL_DIR}/glad/glad_wgl.h
	${EXTERNAL_DIR}/glad/glad.h

	${EXTERNAL_DIR}/glad/khrplatform.h

	${EXTERNAL_DIR}/glad/glad_wgl.c
	${EXTERNAL_DIR}/glad/glad.c
)

source_group(TREE "${EXTERNAL_DIR}/glad" PREFIX "glad" FILES ${GLAD_FILES})
source_group(TREE "${EXTERNAL_DIR}/imgui" PREFIX "imgui" FILES ${IMGUI_FILES})

set(ENGINE_DIR ${MAIN_DIR}/Engine)
set(ENGINE_PUBLIC_DIR ${ENGINE_DIR}/Public)
set(ENGINE_PRIVATE_DIR ${ENGINE_DIR}/Private)
set(ENGINE_HEADER_LIST ${ENGINE_PUBLIC_DIR} ${ENGINE_PRIVATE_DIR})

set(ENGINE_FRAMEWORK_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/Framework/Application.h
	${ENGINE_PUBLIC_DIR}/Framework/Engine.h
	${ENGINE_PUBLIC_DIR}/Framework/Window.h
	${ENGINE_PUBLIC_DIR}/Framework/Player.h
	${ENGINE_PUBLIC_DIR}/Framework/Task.h
	${ENGINE_PUBLIC_DIR}/Framework/TaskWorker.h
	${ENGINE_PUBLIC_DIR}/Framework/ViewController.h

	${ENGINE_PUBLIC_DIR}/Framework/Input/InputHandler.h
	${ENGINE_PUBLIC_DIR}/Framework/Input/InputStorage.h
	${ENGINE_PUBLIC_DIR}/Framework/Input/InputTypes.h

	${ENGINE_PUBLIC_DIR}/Framework/Math/MathTypes.h
)

set(ENGINE_FRAMEWORK_PRIVATE_FILES	
	${ENGINE_PRIVATE_DIR}/Framework/Engine.cpp

	${ENGINE_PRIVATE_DIR}/Framework/InputStorage.cpp
	${ENGINE_PRIVATE_DIR}/Framework/InputTypes.cpp

	${ENGINE_PRIVATE_DIR}/Framework/Player.cpp
	${ENGINE_PRIVATE_DIR}/Framework/ViewController.cpp

	${ENGINE_PRIVATE_DIR}/Framework/CommandList.h

	${ENGINE_PRIVATE_DIR}/Framework/Worker.h
	${ENGINE_PRIVATE_DIR}/Framework/Worker.cpp
	${ENGINE_PRIVATE_DIR}/Framework/TaskWorker.cpp
)

set(ENGINE_PLATFORM_FILES
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32InputHandler.h
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32InputHandler.cpp

	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32Window.h
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32Window.cpp
)

set(ENGINE_RENDERER_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/Renderer/Renderer.h
	${ENGINE_PUBLIC_DIR}/Renderer/RenderGraph.h
	${ENGINE_PUBLIC_DIR}/Renderer/RenderTypes.h

	${ENGINE_PUBLIC_DIR}/Renderer/PipeLine/PipeLine.h
	${ENGINE_PUBLIC_DIR}/Renderer/PipeLine/PipeLine.cpp
	${ENGINE_PUBLIC_DIR}/Renderer/PipeLine/SimpleColor.h
	${ENGINE_PUBLIC_DIR}/Renderer/PipeLine/SimpleColor.cpp
)

set(ENGINE_RENDERER_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/Renderer/Renderer.cpp
	${ENGINE_PRIVATE_DIR}/Renderer/RenderGraph.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/RenderCommandList.h
	${ENGINE_PRIVATE_DIR}/Renderer/RenderCommandList.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/RenderScene.h
	${ENGINE_PRIVATE_DIR}/Renderer/RenderScene.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/RenderWorker.h
	${ENGINE_PRIVATE_DIR}/Renderer/RenderWorker.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/Proxy/PrimitiveProxy.h
	${ENGINE_PRIVATE_DIR}/Renderer/Proxy/PrimitiveProxy.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/PipeLine/PipeLineGraph.h
)

set(ENGINE_RHI_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/RHI/RHICommands.h	
	${ENGINE_PUBLIC_DIR}/RHI/RHICommandList.h
	${ENGINE_PUBLIC_DIR}/RHI/RHIDescriptions.h
	${ENGINE_PUBLIC_DIR}/RHI/RHIResources.h
	${ENGINE_PUBLIC_DIR}/RHI/RHIStates.h
	${ENGINE_PUBLIC_DIR}/RHI/RHISystem.h
)

set(ENGINE_RHI_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/RHI/RHICommands.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHICommandList.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHIResources.cpp
	
	${ENGINE_PRIVATE_DIR}/RHI/RHIExecutor.h
	${ENGINE_PRIVATE_DIR}/RHI/RHIExecutor.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHIFrameExecutor.h
	${ENGINE_PRIVATE_DIR}/RHI/RHIFrameExecutor.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHITaskExecutor.h
	${ENGINE_PRIVATE_DIR}/RHI/RHITaskExecutor.cpp

	${ENGINE_PRIVATE_DIR}/RHI/RHISystem.cpp

	${ENGINE_PRIVATE_DIR}/RHI/RHIWorker.h
	${ENGINE_PRIVATE_DIR}/RHI/RHIWorker.cpp

	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLSystem.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLSystem.cpp
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLResources.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLResources.cpp

	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLContext.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLContext.cpp
)

set(ENGINE_WORLD_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/World/World.h
	${ENGINE_PUBLIC_DIR}/World/WorldContext.h
	${ENGINE_PUBLIC_DIR}/World/Commander.h

	${ENGINE_PUBLIC_DIR}/World/Component.h
	${ENGINE_PUBLIC_DIR}/World/Entity.h
	${ENGINE_PUBLIC_DIR}/World/Node.h
	${ENGINE_PUBLIC_DIR}/World/Scene.h
	${ENGINE_PUBLIC_DIR}/World/System.h
)

set(ENGINE_WORLD_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/World/World.cpp
	${ENGINE_PRIVATE_DIR}/World/WorldContext.cpp

	${ENGINE_PRIVATE_DIR}/World/WorldWorker.h
	${ENGINE_PRIVATE_DIR}/World/WorldWorker.cpp

	${ENGINE_PRIVATE_DIR}/World/Commander.cpp
	${ENGINE_PRIVATE_DIR}/World/Entity.cpp
	${ENGINE_PRIVATE_DIR}/World/Scene.cpp
	${ENGINE_PRIVATE_DIR}/World/System.cpp
)

set(ENGINE_ASSET_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/Asset/AssetSystem.h
	${ENGINE_PUBLIC_DIR}/Asset/AssetLoader.h
	${ENGINE_PUBLIC_DIR}/Asset/AssetFactory.h
	
	${ENGINE_PUBLIC_DIR}/Asset/AssetTypes.h
	${ENGINE_PUBLIC_DIR}/Asset/AssetTypes.cpp

	${ENGINE_PUBLIC_DIR}/Asset/AssetParser.h
	${ENGINE_PUBLIC_DIR}/Asset/AssetParser.cpp

	${ENGINE_PUBLIC_DIR}/Asset/OBJParser.h
	${ENGINE_PUBLIC_DIR}/Asset/OBJParser.cpp

	${ENGINE_PUBLIC_DIR}/Asset/MTLParser.h
	${ENGINE_PUBLIC_DIR}/Asset/MTLParser.cpp

	${ENGINE_PUBLIC_DIR}/Asset/GLSLParser.h
	${ENGINE_PUBLIC_DIR}/Asset/GLSLParser.cpp

	${ENGINE_PUBLIC_DIR}/Asset/StbImageParser.h
	${ENGINE_PUBLIC_DIR}/Asset/StbImageParser.cpp
)

set(ENGINE_ASSET_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/Asset/AssetSystem.cpp
	${ENGINE_PRIVATE_DIR}/Asset/AssetLoader.cpp
	${ENGINE_PRIVATE_DIR}/Asset/AssetFactory.cpp

	${ENGINE_PRIVATE_DIR}/Asset/AssetManager.h
	${ENGINE_PRIVATE_DIR}/Asset/AssetManager.cpp

	${ENGINE_PRIVATE_DIR}/Asset/AssetWorker.h
	${ENGINE_PRIVATE_DIR}/Asset/AssetWorker.cpp
)


source_group(TREE "${ENGINE_PUBLIC_DIR}/Framework" PREFIX "Public\\Framework" FILES ${ENGINE_FRAMEWORK_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/Renderer" PREFIX "Public\\Renderer" FILES ${ENGINE_RENDERER_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/RHI" PREFIX "Public\\RHI" FILES ${ENGINE_RHI_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/World" PREFIX "Public\\World" FILES ${ENGINE_WORLD_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/Asset" PREFIX "Public\\Asset" FILES ${ENGINE_ASSET_PUBLIC_FILES})

source_group(TREE "${ENGINE_PRIVATE_DIR}/Framework" PREFIX "Private\\Framework" FILES ${ENGINE_FRAMEWORK_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/Platform" PREFIX "Private\\Platform" FILES ${ENGINE_PLATFORM_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/Renderer" PREFIX "Private\\Renderer" FILES ${ENGINE_RENDERER_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/RHI" PREFIX "Private\\RHI" FILES ${ENGINE_RHI_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/World" PREFIX "Private\\World" FILES ${ENGINE_WORLD_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/Asset" PREFIX "Private\\Asset" FILES ${ENGINE_ASSET_PRIVATE_FILES})

set(ENGINE_SOURCE_LIST
	${ENGINE_FRAMEWORK_PUBLIC_FILES}
	${ENGINE_RENDERER_PUBLIC_FILES}
	${ENGINE_RHI_PUBLIC_FILES}
	${ENGINE_WORLD_PUBLIC_FILES}
	${ENGINE_ASSET_PUBLIC_FILES}

	${ENGINE_FRAMEWORK_PRIVATE_FILES}
	${ENGINE_PLATFORM_FILES}
	${ENGINE_RENDERER_PRIVATE_FILES}
	${ENGINE_RHI_PRIVATE_FILES}
	${ENGINE_WORLD_PRIVATE_FILES}
	${ENGINE_ASSET_PRIVATE_FILES}
)

set(CLIENT_DIR ${MAIN_DIR}/Client)
set(CLIENT_HEADER_LIST ${CLIENT_DIR}/include)
set(CLIENT_SOURCE_LIST
    ${CLIENT_DIR}/src/main.cpp

	${CLIENT_DIR}/include/DemoApplication.h
	${CLIENT_DIR}/src/DemoApplication.cpp
)

set(HEADER_LIST 
	${ENGINE_HEADER_LIST}
	${CLIENT_HEADER_LIST}
	${EXTERNAL_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
	${ENGINE_SOURCE_LIST}
	${CLIENT_SOURCE_LIST}

	${GLAD_FILES}
	${IMGUI_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC 
	ECSProject 
	MemoryProject 
	ReflectionProject 
	LogProject
	glm
	opengl32
	gdi32
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})