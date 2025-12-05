# Load the debug and release variables
file(GLOB DATA_FILES "${CMAKE_CURRENT_LIST_DIR}/elfutils-*-data.cmake")

foreach(f ${DATA_FILES})
    include(${f})
endforeach()

# Create the targets for all the components
foreach(_COMPONENT ${elfutils_COMPONENT_NAMES} )
    if(NOT TARGET ${_COMPONENT})
        add_library(${_COMPONENT} INTERFACE IMPORTED)
        message(${elfutils_MESSAGE_MODE} "Conan: Component target declared '${_COMPONENT}'")
    endif()
endforeach()

if(NOT TARGET elfutils::elfutils)
    add_library(elfutils::elfutils INTERFACE IMPORTED)
    message(${elfutils_MESSAGE_MODE} "Conan: Target declared 'elfutils::elfutils'")
endif()
# Load the debug and release library finders
file(GLOB CONFIG_FILES "${CMAKE_CURRENT_LIST_DIR}/elfutils-Target-*.cmake")

foreach(f ${CONFIG_FILES})
    include(${f})
endforeach()