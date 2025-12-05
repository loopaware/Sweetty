# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(elfutils_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(elfutils_FRAMEWORKS_FOUND_RELEASE "${elfutils_FRAMEWORKS_RELEASE}" "${elfutils_FRAMEWORK_DIRS_RELEASE}")

set(elfutils_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET elfutils_DEPS_TARGET)
    add_library(elfutils_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET elfutils_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${elfutils_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${elfutils_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:ZLIB::ZLIB;BZip2::BZip2;LibLZMA::LibLZMA;zstd::libzstd_static;elfutils::libelf;elfutils::libdw>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### elfutils_DEPS_TARGET to all of them
conan_package_library_targets("${elfutils_LIBS_RELEASE}"    # libraries
                              "${elfutils_LIB_DIRS_RELEASE}" # package_libdir
                              "${elfutils_BIN_DIRS_RELEASE}" # package_bindir
                              "${elfutils_LIBRARY_TYPE_RELEASE}"
                              "${elfutils_IS_HOST_WINDOWS_RELEASE}"
                              elfutils_DEPS_TARGET
                              elfutils_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "elfutils"    # package_name
                              "${elfutils_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${elfutils_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT elfutils::libasm #############

        set(elfutils_elfutils_libasm_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(elfutils_elfutils_libasm_FRAMEWORKS_FOUND_RELEASE "${elfutils_elfutils_libasm_FRAMEWORKS_RELEASE}" "${elfutils_elfutils_libasm_FRAMEWORK_DIRS_RELEASE}")

        set(elfutils_elfutils_libasm_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET elfutils_elfutils_libasm_DEPS_TARGET)
            add_library(elfutils_elfutils_libasm_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET elfutils_elfutils_libasm_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'elfutils_elfutils_libasm_DEPS_TARGET' to all of them
        conan_package_library_targets("${elfutils_elfutils_libasm_LIBS_RELEASE}"
                              "${elfutils_elfutils_libasm_LIB_DIRS_RELEASE}"
                              "${elfutils_elfutils_libasm_BIN_DIRS_RELEASE}" # package_bindir
                              "${elfutils_elfutils_libasm_LIBRARY_TYPE_RELEASE}"
                              "${elfutils_elfutils_libasm_IS_HOST_WINDOWS_RELEASE}"
                              elfutils_elfutils_libasm_DEPS_TARGET
                              elfutils_elfutils_libasm_LIBRARIES_TARGETS
                              "_RELEASE"
                              "elfutils_elfutils_libasm"
                              "${elfutils_elfutils_libasm_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET elfutils::libasm
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_LIBRARIES_TARGETS}>
                     )

        if("${elfutils_elfutils_libasm_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET elfutils::libasm
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         elfutils_elfutils_libasm_DEPS_TARGET)
        endif()

        set_property(TARGET elfutils::libasm APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET elfutils::libasm APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libasm APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_LIB_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libasm APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET elfutils::libasm APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libasm_COMPILE_OPTIONS_RELEASE}>)


    ########## COMPONENT elfutils::libdw #############

        set(elfutils_elfutils_libdw_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(elfutils_elfutils_libdw_FRAMEWORKS_FOUND_RELEASE "${elfutils_elfutils_libdw_FRAMEWORKS_RELEASE}" "${elfutils_elfutils_libdw_FRAMEWORK_DIRS_RELEASE}")

        set(elfutils_elfutils_libdw_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET elfutils_elfutils_libdw_DEPS_TARGET)
            add_library(elfutils_elfutils_libdw_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET elfutils_elfutils_libdw_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'elfutils_elfutils_libdw_DEPS_TARGET' to all of them
        conan_package_library_targets("${elfutils_elfutils_libdw_LIBS_RELEASE}"
                              "${elfutils_elfutils_libdw_LIB_DIRS_RELEASE}"
                              "${elfutils_elfutils_libdw_BIN_DIRS_RELEASE}" # package_bindir
                              "${elfutils_elfutils_libdw_LIBRARY_TYPE_RELEASE}"
                              "${elfutils_elfutils_libdw_IS_HOST_WINDOWS_RELEASE}"
                              elfutils_elfutils_libdw_DEPS_TARGET
                              elfutils_elfutils_libdw_LIBRARIES_TARGETS
                              "_RELEASE"
                              "elfutils_elfutils_libdw"
                              "${elfutils_elfutils_libdw_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET elfutils::libdw
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_LIBRARIES_TARGETS}>
                     )

        if("${elfutils_elfutils_libdw_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET elfutils::libdw
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         elfutils_elfutils_libdw_DEPS_TARGET)
        endif()

        set_property(TARGET elfutils::libdw APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET elfutils::libdw APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libdw APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_LIB_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libdw APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET elfutils::libdw APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libdw_COMPILE_OPTIONS_RELEASE}>)


    ########## COMPONENT elfutils::libelf #############

        set(elfutils_elfutils_libelf_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(elfutils_elfutils_libelf_FRAMEWORKS_FOUND_RELEASE "${elfutils_elfutils_libelf_FRAMEWORKS_RELEASE}" "${elfutils_elfutils_libelf_FRAMEWORK_DIRS_RELEASE}")

        set(elfutils_elfutils_libelf_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET elfutils_elfutils_libelf_DEPS_TARGET)
            add_library(elfutils_elfutils_libelf_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET elfutils_elfutils_libelf_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'elfutils_elfutils_libelf_DEPS_TARGET' to all of them
        conan_package_library_targets("${elfutils_elfutils_libelf_LIBS_RELEASE}"
                              "${elfutils_elfutils_libelf_LIB_DIRS_RELEASE}"
                              "${elfutils_elfutils_libelf_BIN_DIRS_RELEASE}" # package_bindir
                              "${elfutils_elfutils_libelf_LIBRARY_TYPE_RELEASE}"
                              "${elfutils_elfutils_libelf_IS_HOST_WINDOWS_RELEASE}"
                              elfutils_elfutils_libelf_DEPS_TARGET
                              elfutils_elfutils_libelf_LIBRARIES_TARGETS
                              "_RELEASE"
                              "elfutils_elfutils_libelf"
                              "${elfutils_elfutils_libelf_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET elfutils::libelf
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_LIBRARIES_TARGETS}>
                     )

        if("${elfutils_elfutils_libelf_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET elfutils::libelf
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         elfutils_elfutils_libelf_DEPS_TARGET)
        endif()

        set_property(TARGET elfutils::libelf APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET elfutils::libelf APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libelf APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_LIB_DIRS_RELEASE}>)
        set_property(TARGET elfutils::libelf APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET elfutils::libelf APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${elfutils_elfutils_libelf_COMPILE_OPTIONS_RELEASE}>)


    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET elfutils::elfutils APPEND PROPERTY INTERFACE_LINK_LIBRARIES elfutils::libasm)
    set_property(TARGET elfutils::elfutils APPEND PROPERTY INTERFACE_LINK_LIBRARIES elfutils::libdw)
    set_property(TARGET elfutils::elfutils APPEND PROPERTY INTERFACE_LINK_LIBRARIES elfutils::libelf)

########## For the modules (FindXXX)
set(elfutils_LIBRARIES_RELEASE elfutils::elfutils)
