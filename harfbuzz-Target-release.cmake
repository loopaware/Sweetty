# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(harfbuzz_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(harfbuzz_FRAMEWORKS_FOUND_RELEASE "${harfbuzz_FRAMEWORKS_RELEASE}" "${harfbuzz_FRAMEWORK_DIRS_RELEASE}")

set(harfbuzz_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET harfbuzz_DEPS_TARGET)
    add_library(harfbuzz_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET harfbuzz_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${harfbuzz_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${harfbuzz_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:Freetype::Freetype;glib::glib>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### harfbuzz_DEPS_TARGET to all of them
conan_package_library_targets("${harfbuzz_LIBS_RELEASE}"    # libraries
                              "${harfbuzz_LIB_DIRS_RELEASE}" # package_libdir
                              "${harfbuzz_BIN_DIRS_RELEASE}" # package_bindir
                              "${harfbuzz_LIBRARY_TYPE_RELEASE}"
                              "${harfbuzz_IS_HOST_WINDOWS_RELEASE}"
                              harfbuzz_DEPS_TARGET
                              harfbuzz_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "harfbuzz"    # package_name
                              "${harfbuzz_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${harfbuzz_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Release ########################################
    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Release>:${harfbuzz_OBJECTS_RELEASE}>
                 $<$<CONFIG:Release>:${harfbuzz_LIBRARIES_TARGETS}>
                 )

    if("${harfbuzz_LIBS_RELEASE}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET harfbuzz::harfbuzz
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     harfbuzz_DEPS_TARGET)
    endif()

    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Release>:${harfbuzz_LINKER_FLAGS_RELEASE}>)
    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Release>:${harfbuzz_INCLUDE_DIRS_RELEASE}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Release>:${harfbuzz_LIB_DIRS_RELEASE}>)
    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Release>:${harfbuzz_COMPILE_DEFINITIONS_RELEASE}>)
    set_property(TARGET harfbuzz::harfbuzz
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Release>:${harfbuzz_COMPILE_OPTIONS_RELEASE}>)

########## For the modules (FindXXX)
set(harfbuzz_LIBRARIES_RELEASE harfbuzz::harfbuzz)
