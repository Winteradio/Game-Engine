# ------ Set for ExternalProject ---------------- #
MESSAGE( STATUS "Setting for ExternalProject")

INCLUDE( ExternalProject )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Variables for Dependency ----------- #
MESSAGE( STATUS "Setting Variables for Dependency")

SET( MAIN_DIR ${CMAKE_SOURCE_DIR} )

SET( SRC_DIR ${MAIN_DIR}/src )
SET( THIRD_PARTY_DIR ${MAIN_DIR}/third-party )

SET( SRCS )
SET( INCS )

SET( DEP )
SET( DEP_INCLUDE_DIR )
SET( DEP_LIB_DIR )
SET( DEP_LIBS )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



#### Window SDK ---------------------------------- #
MESSAGE( STATUS "Window - Linking : WINDOW SDK")

SET( WINDOWSDK_INCLUDE_DIR
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\shared"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\um"
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\winrt
	")

SET( WINDOWSDK_LIB_DIR
	"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\um"
	)

SET( WINDOWSDK_LIBS winMM.lib Pdh.lib)

LIST( APPEND DEP_INCLUDE_DIR ${WINDOWSDK_INCLUDE_DIR} )
LIST( APPEND DEP_LIB_DIR ${WINDOWSDK_LIB_DIR} )
LIST( APPEND DEP_LIBS ${WINDOWSDK_LIBS} )

MESSAGE( STATUS "WindowSDK - Done")
#### Window SDK ---------------------------------- #



#### DIRECTX11 ---------------------------------- #
MESSAGE( STATUS "Directx11 - Linking : DIRECTX SDK")

SET( D11_INCLUDE_DIR
	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"
	)
SET( D11_LIB_DIR
	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64"
	)
SET( D11_LIBS d3d11.lib d3dx11.lib d3dx10.lib dxgi.lib dxguid.lib d3dcompiler.lib dinput8.lib )

LIST( APPEND DEP_INCLUDE_DIR ${D11_INCLUDE_DIR} )
LIST( APPEND DEP_LIB_DIR ${D11_LIB_DIR} )
LIST( APPEND DEP_LIBS ${D11_LIBS} )

MESSAGE( STATUS "Directx11 - Done")
#### DIRECTX11 ---------------------------------- #



#### YAML-CPP ----------------------------------- #
MESSAGE( STATUS "yaml-cpp - Linking ... ")

SET( YAML_DIR ${THIRD_PARTY_DIR}/yaml-cpp )

EXTERNALPROJECT_ADD(
	yaml-cpp-project
	GIT_REPOSITORY GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG "yaml-cpp-0.7.0"

	PREFIX ${CMAKE_BINARY_DIR}/yaml-cpp
	INSTALL_DIR ${CMAKE_BINARY_DIR}/yaml-cpp
	SOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/yaml-cpp
	BINARY_DIR ${CMAKE_BINARY_DIR}/yaml-cpp/build
	UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
	CMAKE_ARGS -DYAML_CPP_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
	)

ADD_LIBRARY( yaml-cpp STATIC IMPORTED )
SET_TARGET_PROPERTIES(yaml-cpp PROPERTIES IMPORTED_LOCATION  ${CMAKE_BINARY_DIR}/yaml-cpp/build/Debug/yaml-cppd.lib
)
LIST( APPEND DEP yaml-cpp-project )
LIST( APPEND DEP_INCLUDE_DIR ${YAML_DIR}/include )
LIST( APPEND DEP_LIBS yaml-cpp )

MESSAGE( STATUS "yaml-cpp - Done")
#### YAML-CPP ----------------------------------- #



#### UUID --------------------------------------- #
MESSAGE( STATUS "RPCRT4-UUID - Linking ... ")

SET( UUID_LIBS Rpcrt4.lib )

LIST( APPEND DEP_LIBS ${UUID_LIBS} )

MESSAGE( STATUS "RPCRT4-UUID - Done")
#### UUID --------------------------------------- #



#### IMGUI -------------------------------------- #
MESSAGE( STATUS "IMGUI - Linking ... ")

SET( IMGUI_DIR ${CMAKE_SOURCE_DIR}/third-party/imgui )

SET( IMGUIS
		${IMGUI_DIR}/imconfig.h
		${IMGUI_DIR}/imgui.h ${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_impl_dx11.h ${IMGUI_DIR}/imgui_impl_dx11.cpp
		${IMGUI_DIR}/imgui_impl_win32.h ${IMGUI_DIR}/imgui_impl_win32.cpp
		${IMGUI_DIR}/imgui_internal.h
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/imstb_rectpack.h
		${IMGUI_DIR}/imstb_textedit.h
		${IMGUI_DIR}/imstb_truetype.h
	)

LIST( APPEND SRCS ${IMGUIS} )
LIST( APPEND INCS ${IMGUI_DIR} )

MESSAGE( STATUS "IMGUI - Done ")
#### IMGUI -------------------------------------- #