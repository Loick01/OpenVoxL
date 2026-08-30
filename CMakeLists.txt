cmake_minimum_required(VERSION 3.10)

project(openvoxl VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw-3.5.1)
add_subdirectory(external/glm)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-1.90.4)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/src/*.cpp)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR}/include ${IMGUI_DIR}/backends)

find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

target_sources(imgui PRIVATE 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set_target_properties(glad imgui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(openvoxl ${SOURCES})

target_include_directories(openvoxl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

target_link_libraries(openvoxl glfw glad glm::glm imgui)