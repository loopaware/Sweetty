########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(harfbuzz_FIND_QUIETLY)
    set(harfbuzz_MESSAGE_MODE VERBOSE)
else()
    set(harfbuzz_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/harfbuzzTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${harfbuzz_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(harfbuzz_VERSION_STRING "8.3.0")
set(harfbuzz_INCLUDE_DIRS ${harfbuzz_INCLUDE_DIRS_RELEASE} )
set(harfbuzz_INCLUDE_DIR ${harfbuzz_INCLUDE_DIRS_RELEASE} )
set(harfbuzz_LIBRARIES ${harfbuzz_LIBRARIES_RELEASE} )
set(harfbuzz_DEFINITIONS ${harfbuzz_DEFINITIONS_RELEASE} )


# Definition of extra CMake variables from cmake_extra_variables


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${harfbuzz_BUILD_MODULES_PATHS_RELEASE} )
    message(${harfbuzz_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


