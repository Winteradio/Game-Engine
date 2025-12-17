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
set(HEADER_DIR ${MAIN_DIR}/include)
set(SOURCE_DIR ${MAIN_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

set(HEADER_LIST 
	${HEADER_DIR}
	${EXTERNAL_DIR}
)

target_sources(${PROJECT_NAME} PRIVATE
    ${SOURCE_DIR}/main.cpp

	${EXTERNAL_DIR}/glad/src/glad_wgl.c
	${EXTERNAL_DIR}/glad/src/glad.c
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