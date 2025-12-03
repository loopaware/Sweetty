cmake_minimum_required(VERSION 3.10)

project(sweetty-gl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(FetchContent)

# --- Find or Fetch ZLIB ---
find_package(ZLIB QUIET)
if (NOT ZLIB_FOUND)
    FetchContent_Declare(
        zlib_extern
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG        v1.2.13 # Use a specific version or commit
    )
    FetchContent_MakeAvailable(zlib_extern)
    # Ensure zlib target is available if fetched
    add_library(ZLIB::zlib ALIAS zlib_extern)
endif()

# --- Fetch FreeType ---
# FreeType has its own CMakeLists.txt and should build itself
FetchContent_Declare(
    freetype_extern
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG        VER-2-13-2 # Use a specific version or commit
    # Freetype's CMake can try to find dependencies like zlib, bzip2, png
    # Ensure zlib is available to it via system or fetched externally
)
FetchContent_MakeAvailable(freetype_extern)

FetchContent_Declare(
    harfbuzz_extern
    GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git
    GIT_TAG        8.3.0 # Use a specific version or commit
)
FetchContent_MakeAvailable(harfbuzz_extern)

# --- Fetch GLM ---
FetchContent_Declare(
    glm_extern
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8 # Use a specific version or commit
)
FetchContent_MakeAvailable(glm_extern)


find_package(Qt5 5.15 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(sweetty-gl src/main.cpp terminal_buffer.cpp)

target_link_libraries(sweetty-gl PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
    freetype
    harfbuzz
)

if (ZLIB_FOUND)
    target_link_libraries(sweetty-gl PRIVATE ZLIB::ZLIB)
else()
    target_link_libraries(sweetty-gl PRIVATE zlib_extern)
endif()

target_include_directories(sweetty-gl PRIVATE
    ${glm_extern_SOURCE_DIR}
)
