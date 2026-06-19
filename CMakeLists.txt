cmake_minimum_required(VERSION 3.16)
project(Acroy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(Dependencies/glfw)
add_subdirectory(Dependencies/glad)
add_subdirectory(Dependencies/stb)
add_subdirectory(Dependencies/tinyobjloader)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
