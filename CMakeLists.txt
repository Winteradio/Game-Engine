# ------ Set Options for Projects --------------- #
MESSAGE( STATUS "Set Options for Project")

## CMake minimun version required
CMAKE_MINIMUM_REQUIRED( VERSION 3.11 )

## Make Project Name
SET( PROJECT_NAME "NewEngine" )
## Project name
PROJECT( ${PROJECT_NAME} )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Variables -------------------------- #
MESSAGE( STATUS "Set Variables")
## Set Compiler by Generator
IF ( CMAKE_GENERATOR MATCHES "Visual Studio" )
	MESSAGE( STATUS "${CMAKE_GENERATOR}" )
	SET( CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe" )
	SET( CMAKE_C_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe" )
ENDIF()
# Set Build Type and Compiler Flags
SET(CMAKE_BUILD_TYPE Debug)
if( MSVC )
	MESSAGE( STATUS "Compiler is ${CMAKE_CXX_COMPILER_ID}")
    # Use multi-process debugging
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /MP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi /MP")

    # Use Windbg as the debugger
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG:FULL")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DEBUG:FULL")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} /DEBUG /INCREMENTAL:NO")
endif()

## Set CXX Compiler Version
SET( CMAKE_CXX_STANDARD 17 )
## Set Exe file place
SET( RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Files ------------------------------ #
MESSAGE( STATUS "Set Source File")

## Include CMake's Dependency file
INCLUDE( Dependency.cmake)

## Source Directories
SET( ENGINE_DIR ${SRC_DIR}/Engine )
	SET( CORE_DIR ${ENGINE_DIR}/Core )
	SET( ENTITY_DIR ${ENGINE_DIR}/Entity )
	SET( HANDLER_DIR ${ENGINE_DIR}/Handler )
	SET( MANAGER_DIR ${ENGINE_DIR}/Manager )
	SET( SYSTEM_DIR ${ENGINE_DIR}/System )

SET( API_DIR ${SRC_DIR}/API )
	SET( DIRECTX_DIR ${API_DIR}/DirectX )
	SET( IMGUI_DIR ${API_DIR}/ImGui )
	SET( UUID_DIR ${API_DIR}/UUID )
	SET( WINDOW_DIR ${API_DIR}/Window )

SET( UTILS_DIR ${SRC_DIR}/Utils )

## Source Files
LIST( APPEND SRCS
	${SRC_DIR}/main.cpp

	## Engine
		## Core
		${CORE_DIR}/Core.h ${CORE_DIR}/Core.cpp
		${CORE_DIR}/Log.h ${CORE_DIR}/Log.cpp
		${CORE_DIR}/Name.h ${CORE_DIR}/Name.cpp
		${CORE_DIR}/Timer.h ${CORE_DIR}/Timer.cpp
		## Entity
		${ENTITY_DIR}/Entity.h ${ENTITY_DIR}/Entity.cpp
		${ENTITY_DIR}/Scene.h ${ENTITY_DIR}/Scene.cpp
		${ENTITY_DIR}/Node.h
		${ENTITY_DIR}/Component.h
		## Handler
		${HANDLER_DIR}/Handler.h
		${HANDLER_DIR}/AssetHandler.h ${HANDLER_DIR}/AssetHandler.cpp
		${HANDLER_DIR}/FileHandler.h ${HANDLER_DIR}/FileHandler.cpp
		${HANDLER_DIR}/LogUIHandler.h ${HANDLER_DIR}/LogUIHandler.cpp
		${HANDLER_DIR}/MainUIHandler.h ${HANDLER_DIR}/MainUIHandler.cpp
		${HANDLER_DIR}/MenuBarUIHandler.h ${HANDLER_DIR}/MenuBarUIHandler.cpp
		${HANDLER_DIR}/PropertyUIHandler.h	${HANDLER_DIR}/PropertyUIHandler.cpp
		${HANDLER_DIR}/StateUIHandler.h ${HANDLER_DIR}/StateUIHandler.cpp
		## Manager
		${MANAGER_DIR}/Manager.h
		${MANAGER_DIR}/ComponentManager.h ${MANAGER_DIR}/ComponentManager.cpp
		${MANAGER_DIR}/NodeManager.h ${MANAGER_DIR}/NodeManager.cpp
		${MANAGER_DIR}/SceneManager.h ${MANAGER_DIR}/SceneManager.cpp
		${MANAGER_DIR}/SystemManager.h ${MANAGER_DIR}/SystemManager.cpp
		## System
		${SYSTEM_DIR}/ISystem.h
		${SYSTEM_DIR}/CameraSystem.h ${SYSTEM_DIR}/CameraSystem.cpp
		${SYSTEM_DIR}/RenderSystem.h ${SYSTEM_DIR}/RenderSystem.cpp

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
		${WINDOW_DIR}/IWindow.h
		${WINDOW_DIR}/WinAPI.h ${WINDOW_DIR}/WinAPI.cpp

	## Others..
		## Utils
		${UTILS_DIR}/Utils.h
	)

## Header Files
LIST( APPEND INCS

	## Engine
		${ENGINE_DIR}
			${CORE_DIR}
			${ENTITY_DIR}
			${HANDLER_DIR}
			${SYSTEM_DIR}
			${MANAGER_DIR}

	## API
		${API_DIR}
			${DIRECTX_DIR}
			${IMGUI_DIR}
			${UUID_DIR}
			${WINDOW_DIR}

	## Other..
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