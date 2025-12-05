# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/harfbuzz-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${harfbuzz_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${harfbuzz_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET harfbuzz::harfbuzz)
    add_library(harfbuzz::harfbuzz INTERFACE IMPORTED)
    message(${harfbuzz_MESSAGE_MODE} "Conan: Target declared 'harfbuzz::harfbuzz'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/harfbuzz-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()