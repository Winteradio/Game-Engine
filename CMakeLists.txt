# ------ Set the basic options for the GameEngine project ------- #
message(STATUS "# Set the basic options for the GameEngine project")

cmake_minimum_required(VERSION 3.20)
project(GameEngine LANGUAGES CXX)

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

# ------ Set the properties the executable program ------ #
set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

set(GLAD_INCLUDE_FILES
	${EXTERNAL_DIR}/glad/include/glad/glad_wgl.h
	${EXTERNAL_DIR}/glad/include/glad/glad.h

	${EXTERNAL_DIR}/glad/include/KHR/khrplatform.h
)

set(GLAD_SOURCE_FILES
	${EXTERNAL_DIR}/glad/src/glad_wgl.c
	${EXTERNAL_DIR}/glad/src/glad.c
)

source_group(TREE "${EXTERNAL_DIR}/glad" PREFIX "glad" FILES ${GLAD_INCLUDE_FILES})
source_group(TREE "${EXTERNAL_DIR}/glad" PREFIX "glad" FILES ${GLAD_SOURCE_FILES})

set(ENGINE_DIR ${MAIN_DIR}/Engine)
set(ENGINE_PUBLIC_DIR ${ENGINE_DIR}/Public)
set(ENGINE_PRIVATE_DIR ${ENGINE_DIR}/Private)
set(ENGINE_HEADER_LIST ${ENGINE_PUBLIC_DIR} ${ENGINE_PRIVATE_DIR})

set(ENGINE_FRAMEWORK_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/Framework/Application.h
	${ENGINE_PUBLIC_DIR}/Framework/Engine.h
	${ENGINE_PUBLIC_DIR}/Framework/Window.h

	${ENGINE_PUBLIC_DIR}/Framework/Input/InputHandler.h
	${ENGINE_PUBLIC_DIR}/Framework/Input/InputStorage.h
	${ENGINE_PUBLIC_DIR}/Framework/Input/InputTypes.h

	${ENGINE_PUBLIC_DIR}/Framework/Math/MathTypes.h
)

set(ENGINE_FRAMEWORK_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/Framework/CommandList.h
	
	${ENGINE_PRIVATE_DIR}/Framework/Engine.cpp
	${ENGINE_PRIVATE_DIR}/Framework/InputStorage.cpp
	${ENGINE_PRIVATE_DIR}/Framework/InputTypes.cpp

	${ENGINE_PRIVATE_DIR}/Framework/Worker.h
	${ENGINE_PRIVATE_DIR}/Framework/Worker.cpp
)

set(ENGINE_PLATFORM_FILES
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32InputHandler.h
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32InputHandler.cpp

	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32Window.h
	${ENGINE_PRIVATE_DIR}/Platform/Win32/Win32Window.cpp
)

set(ENGINE_RENDERER_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/Renderer/Renderer.h
	${ENGINE_PUBLIC_DIR}/Renderer/RenderCommands.h
	${ENGINE_PUBLIC_DIR}/Renderer/RenderCommandList.h
)

set(ENGINE_RENDERER_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/Renderer/RenderCommands.cpp
	${ENGINE_PRIVATE_DIR}/Renderer/RenderCommandList.cpp
	${ENGINE_PRIVATE_DIR}/Renderer/Renderer.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/RenderContext.h
	${ENGINE_PRIVATE_DIR}/Renderer/RenderContext.cpp

	${ENGINE_PRIVATE_DIR}/Renderer/RenderWorker.h
	${ENGINE_PRIVATE_DIR}/Renderer/RenderWorker.cpp
)

set(ENGINE_RHI_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/RHI/RHICommands.h	
	${ENGINE_PUBLIC_DIR}/RHI/RHICommandList.h
	${ENGINE_PUBLIC_DIR}/RHI/RHIDefinitions.h
	${ENGINE_PUBLIC_DIR}/RHI/RHIResources.h
	${ENGINE_PUBLIC_DIR}/RHI/RHITypes.h
)

set(ENGINE_RHI_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/RHI/RHICommands.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHICommandList.cpp
	${ENGINE_PRIVATE_DIR}/RHI/RHIDefinitions.cpp

	${ENGINE_PRIVATE_DIR}/RHI/RHIContext.h
	${ENGINE_PRIVATE_DIR}/RHI/RHIDevice.h
	
	${ENGINE_PRIVATE_DIR}/RHI/RHISystem.h
	${ENGINE_PRIVATE_DIR}/RHI/RHISystem.cpp

	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLResources.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/GLResources.cpp

	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLContext.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLContext.cpp
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLDevice.h
	${ENGINE_PRIVATE_DIR}/RHI/OpenGL/Win32/WGLDevice.cpp
)

set(ENGINE_WORLD_PUBLIC_FILES
	${ENGINE_PUBLIC_DIR}/World/World.h
	${ENGINE_PUBLIC_DIR}/World/WorldCommands.h
	${ENGINE_PUBLIC_DIR}/World/WorldCommandList.h
)

set(ENGINE_WORLD_PRIVATE_FILES
	${ENGINE_PRIVATE_DIR}/World/World.cpp
	${ENGINE_PRIVATE_DIR}/World/WorldCommands.cpp
	${ENGINE_PRIVATE_DIR}/World/WorldCommandList.cpp

	${ENGINE_PRIVATE_DIR}/World/WorldWorker.h
	${ENGINE_PRIVATE_DIR}/World/WorldWorker.cpp
)

source_group(TREE "${ENGINE_PUBLIC_DIR}/Framework" PREFIX "Public\\Framework" FILES ${ENGINE_FRAMEWORK_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/Renderer" PREFIX "Public\\Renderer" FILES ${ENGINE_RENDERER_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/RHI" PREFIX "Public\\RHI" FILES ${ENGINE_RHI_PUBLIC_FILES})
source_group(TREE "${ENGINE_PUBLIC_DIR}/World" PREFIX "Public\\World" FILES ${ENGINE_WORLD_PUBLIC_FILES})

source_group(TREE "${ENGINE_PRIVATE_DIR}/Framework" PREFIX "Private\\Framework" FILES ${ENGINE_FRAMEWORK_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/Platform" PREFIX "Public\\RHI" FILES ${ENGINE_PLATFORM_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/Renderer" PREFIX "Private\\Renderer" FILES ${ENGINE_RENDERER_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/RHI" PREFIX "Private\\RHI" FILES ${ENGINE_RHI_PRIVATE_FILES})
source_group(TREE "${ENGINE_PRIVATE_DIR}/World" PREFIX "Private\\World" FILES ${ENGINE_WORLD_PRIVATE_FILES})

set(ENGINE_SOURCE_LIST
	${ENGINE_FRAMEWORK_PUBLIC_FILES}
	${ENGINE_RENDERER_PUBLIC_FILES}
	${ENGINE_RHI_PUBLIC_FILES}
	${ENGINE_WORLD_PUBLIC_FILES}

	${ENGINE_FRAMEWORK_PRIVATE_FILES}
	${ENGINE_PLATFORM_FILES}
	${ENGINE_RENDERER_PRIVATE_FILES}
	${ENGINE_RHI_PRIVATE_FILES}
	${ENGINE_WORLD_PRIVATE_FILES}
)

set(CLIENT_DIR ${MAIN_DIR}/Client)
set(CLIENT_HEADER_LIST ${CLIENT_DIR}/include)
set(CLIENT_SOURCE_LIST
    ${CLIENT_DIR}/src/main.cpp
)

set(HEADER_LIST 
	${ENGINE_HEADER_LIST}
	${CLIENT_HEADER_LIST}
	${EXTERNAL_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
	${ENGINE_SOURCE_LIST}
	${CLIENT_SOURCE_LIST}

	${GLAD_INCLUDE_FILES}
	${GLAD_SOURCE_FILES}
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