cmake_minimum_required(VERSION 3.10)

project(sweetty-gl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.15 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(sweetty-gl src/main.cpp)

target_link_libraries(sweetty-gl PRIVATE Qt5::Widgets Qt5::OpenGL)
