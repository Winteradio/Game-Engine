# ------ Set Variables -------------------------- #
MESSAGE( STATUS "Set Variables")

## Make Project Name
SET( PROJECT_NAME "Example" )
## Set Compiler
SET( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
SET( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
SET( CMAKE_BUILD_TYPE Debug )
## Set CXX Compiler Version
SET( CMAKE_CXX_STANDARD 17 )
## Set Exe file place
SET( RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Options for Projects --------------- #
MESSAGE( STATUS "Set Options for Project")

## CMake minimun version required
CMAKE_MINIMUM_REQUIRED( VERSION 3.11 )
## Project name
PROJECT( ${PROJECT_NAME} )
## Set compile options

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Files ------------------------------ #
MESSAGE( STATUS "Set Source File")

## Include CMake's Dependency file
INCLUDE( Dependency.cmake)

## Source Directories
SET( ENGINE_DIR ${SRC_DIR}/Engine )
	SET( COREENGINE_DIR ${ENGINE_DIR}/CoreEngine )
	SET( PHYSICSENGINE_DIR ${ENGINE_DIR}/PhysicsEngine )
	SET( RENDERINGENGINE_DIR ${ENGINE_DIR}/RenderingEngine )

	SET( FILEDIALOG_DIR ${ENGINE_DIR}/FileDialog )
	SET( LAYER_DIR ${ENGINE_DIR}/Layer )
	SET( MANAGER_DIR ${ENGINE_DIR}/Manager )
	SET( RESOURCE_DIR ${ENGINE_DIR}/Resource )
	SET( TEXTEDITOR_DIR ${ENGINE_DIR}/TextEditor)

SET( DIRECTX_DIR ${SRC_DIR}/DirectX )
SET( IMGUI_DIR ${SRC_DIR}/ImGui )
SET( UUID_DIR ${SRC_DIR}/UUID )
SET( WINDOW_DIR ${SRC_DIR}/Window )

SET( UTILS_DIR ${SRC_DIR}/Utils )

## Source Files
LIST( APPEND SRCS
	${SRC_DIR}/main.cpp
## My Engine
	## Engine
		## Core Engine
		${COREENGINE_DIR}/IWindow.h
		${COREENGINE_DIR}/Log.h ${COREENGINE_DIR}/Log.cpp
		${COREENGINE_DIR}/Timer.h
		${COREENGINE_DIR}/CoreEngine.h ${COREENGINE_DIR}/CoreEngine.cpp

		## Physics Engine
		${PHYSICSENGINE_DIR}/PhysicsEngine.h ${PHYSICSENGINE_DIR}/PhysicsEngine.cpp

		## Rendering Engine
		${RENDERINGENGINE_DIR}/RenderingEngine.h ${RENDERINGENGINE_DIR}/RenderingEngine.cpp
		${RENDERINGENGINE_DIR}/RenderingStatus.h ${RENDERINGENGINE_DIR}/RenderingStatus.cpp

		## Resource
		${RESOURCE_DIR}/ResourceManager.h ${RESOURCE_DIR}/ResourceManager.cpp
		${RESOURCE_DIR}/IResource.h
		${RESOURCE_DIR}/IMovableResource.h
			${RESOURCE_DIR}/Light.h ${RESOURCE_DIR}/Light.cpp
			${RESOURCE_DIR}/Camera.h ${RESOURCE_DIR}/Camera.cpp
			${RESOURCE_DIR}/IBundleResource.h
				${RESOURCE_DIR}/Scene.h ${RESOURCE_DIR}/Scene.cpp
				${RESOURCE_DIR}/Component.h ${RESOURCE_DIR}/Component.cpp
		${RESOURCE_DIR}/IFileResource.h
			${RESOURCE_DIR}/Shader.h ${RESOURCE_DIR}/Shader.cpp
			${RESOURCE_DIR}/Mesh.h ${RESOURCE_DIR}/Mesh.cpp
			${RESOURCE_DIR}/Texture.h ${RESOURCE_DIR}/Texture.cpp
			${RESOURCE_DIR}/Script.h ${RESOURCE_DIR}/Script.cpp

		## Layer
		${LAYER_DIR}/LayerManager.h ${LAYER_DIR}/LayerManager.cpp
		${LAYER_DIR}/ILayer.h
			${LAYER_DIR}/GameLayer.h ${LAYER_DIR}/GameLayer.cpp
		${LAYER_DIR}/IGuiLayer.h
			${LAYER_DIR}/LogGuiLayer.h ${LAYER_DIR}/LogGuiLayer.cpp
			${LAYER_DIR}/ResourceGuiLayer.h ${LAYER_DIR}/ResourceGuiLayer.cpp
			${LAYER_DIR}/IBrowserGuiLayer.h
				${LAYER_DIR}/MenuBrowserGuiLayer.h ${LAYER_DIR}/MenuBrowserGuiLayer.cpp
				${LAYER_DIR}/PropertyBrowserGuiLayer.h ${LAYER_DIR}/PropertyBrowserGuiLayer.cpp
				${LAYER_DIR}/FileBrowserGuiLayer.h ${LAYER_DIR}/FileBrowserGuiLayer.cpp
				${LAYER_DIR}/SettingBrowserGuiLayer.h ${LAYER_DIR}/SettingBrowserGuiLayer.cpp

		## File Dialog
		${FILEDIALOG_DIR}/FileDialog.h ${FILEDIALOG_DIR}/FileDialog.cpp
		${FILEDIALOG_DIR}/ShaderManager.h ${FILEDIALOG_DIR}/ShaderManager.cpp
		${FILEDIALOG_DIR}/TextureManager.h ${FILEDIALOG_DIR}/TextureManager.cpp
		${FILEDIALOG_DIR}/ProjectManager.h ${FILEDIALOG_DIR}/ProjectManager.cpp

## API
	## DirectX
	${DIRECTX_DIR}/DirectXUtils.h
	${DIRECTX_DIR}/DirectXAPI.h ${DIRECTX_DIR}/DirectXAPI.cpp
	${DIRECTX_DIR}/DirectXInput.h ${DIRECTX_DIR}/DirectXInput.cpp

	## ImGui
	${IMGUI_DIR}/ImGuiUtils.h
	${IMGUI_DIR}/ImGuiAPI.h ${IMGUI_DIR}/ImGuiAPI.cpp

	## UUID
	${UUID_DIR}/MyUUID.h ${UUID_DIR}/MyUUID.cpp

	## Window
	${WINDOW_DIR}/WinAPI.h ${WINDOW_DIR}/WinAPI.cpp
	)
## Header Files
LIST( APPEND INCS

	${ENGINE_DIR}
		${COREENGINE_DIR}
		${PHYSICSENGINE_DIR}
		${RENDERINGENGINE_DIR}

		${FILEDIALOG_DIR}
		${LAYER_DIR}
		${MANAGER_DIR}
		${RESOURCE_DIR}
		${TEXTEDITOR_DIR}

	${DIRECTX_DIR}
	${IMGUI_DIR}
	${UUID_DIR}
	${WINDOW_DIR}

	${UTILS_DIR}
	)

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Set Options for Target Files")

## Include Header Directories for Target Files
ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRCS} )

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${INCS} ${DEP_INCLUDE_DIR} )
TARGET_LINK_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC ${DEP_LIBS} )
ADD_DEPENDENCIES( ${PROJECT_NAME} yaml-cpp-project)

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #