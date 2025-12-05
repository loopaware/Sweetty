message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(harfbuzz)
find_package(freetype)
find_package(ZLIB)
find_package(GTest)

set(CONANDEPS_LEGACY  harfbuzz::harfbuzz  Freetype::Freetype  ZLIB::ZLIB  gtest::gtest )