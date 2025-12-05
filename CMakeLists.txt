cmake_minimum_required(VERSION 3.15)
project(sweetty-gl)

# Load the Conan toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_FOUND}")
    message(STATUS "Using ccache to accelerate compilation.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Fetch GLM ---
include(FetchContent)
FetchContent_Declare(
    glm_extern
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8 # Use a specific version or commit
)
FetchContent_MakeAvailable(glm_extern)

find_package(Qt5 5.15 COMPONENTS Widgets OpenGL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Freetype REQUIRED)
find_package(harfbuzz REQUIRED)

add_executable(sweetty-gl src/main.cpp src/pty_process.cpp src/font_manager.cpp src/terminal_buffer.cpp src/ansi_parser.cpp src/terminal_widget.cpp src/renderer.cpp)

target_link_libraries(sweetty-gl PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
    Freetype::Freetype
    harfbuzz::harfbuzz
    ZLIB::ZLIB
)

target_include_directories(sweetty-gl PRIVATE
    ${glm_extern_SOURCE_DIR}
    src # Add the project's src/ directory for local headers
)

enable_testing()

find_package(GTest REQUIRED)

add_executable(unit_tests test/main.cpp)
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
