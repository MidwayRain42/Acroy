cmake_minimum_required(VERSION 3.24)
project(Sandbox)

add_executable(Sandbox Main.cpp
    CameraController.hpp
    Game.hpp
    Game.cpp)

target_link_libraries(Sandbox PRIVATE Acroy)
