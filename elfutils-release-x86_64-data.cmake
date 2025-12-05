########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND elfutils_COMPONENT_NAMES elfutils::libelf elfutils::libdw elfutils::libasm)
list(REMOVE_DUPLICATES elfutils_COMPONENT_NAMES)
if(DEFINED elfutils_FIND_DEPENDENCY_NAMES)
  list(APPEND elfutils_FIND_DEPENDENCY_NAMES LibLZMA zstd ZLIB BZip2)
  list(REMOVE_DUPLICATES elfutils_FIND_DEPENDENCY_NAMES)
else()
  set(elfutils_FIND_DEPENDENCY_NAMES LibLZMA zstd ZLIB BZip2)
endif()
set(LibLZMA_FIND_MODE "NO_MODULE")
set(zstd_FIND_MODE "NO_MODULE")
set(ZLIB_FIND_MODE "NO_MODULE")
set(BZip2_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(elfutils_PACKAGE_FOLDER_RELEASE "/home/fredrick/.conan2/p/b/elfutac9195ff82751/p")
set(elfutils_BUILD_MODULES_PATHS_RELEASE )


set(elfutils_INCLUDE_DIRS_RELEASE )
set(elfutils_RES_DIRS_RELEASE )
set(elfutils_DEFINITIONS_RELEASE )
set(elfutils_SHARED_LINK_FLAGS_RELEASE )
set(elfutils_EXE_LINK_FLAGS_RELEASE )
set(elfutils_OBJECTS_RELEASE )
set(elfutils_COMPILE_DEFINITIONS_RELEASE )
set(elfutils_COMPILE_OPTIONS_C_RELEASE )
set(elfutils_COMPILE_OPTIONS_CXX_RELEASE )
set(elfutils_LIB_DIRS_RELEASE "${elfutils_PACKAGE_FOLDER_RELEASE}/lib")
set(elfutils_BIN_DIRS_RELEASE )
set(elfutils_LIBRARY_TYPE_RELEASE STATIC)
set(elfutils_IS_HOST_WINDOWS_RELEASE 0)
set(elfutils_LIBS_RELEASE asm dw elf)
set(elfutils_SYSTEM_LIBS_RELEASE dl pthread)
set(elfutils_FRAMEWORK_DIRS_RELEASE )
set(elfutils_FRAMEWORKS_RELEASE )
set(elfutils_BUILD_DIRS_RELEASE )
set(elfutils_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(elfutils_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${elfutils_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${elfutils_COMPILE_OPTIONS_C_RELEASE}>")
set(elfutils_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${elfutils_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${elfutils_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${elfutils_EXE_LINK_FLAGS_RELEASE}>")


set(elfutils_COMPONENTS_RELEASE elfutils::libelf elfutils::libdw elfutils::libasm)
########### COMPONENT elfutils::libasm VARIABLES ############################################

set(elfutils_elfutils_libasm_INCLUDE_DIRS_RELEASE )
set(elfutils_elfutils_libasm_LIB_DIRS_RELEASE "${elfutils_PACKAGE_FOLDER_RELEASE}/lib")
set(elfutils_elfutils_libasm_BIN_DIRS_RELEASE )
set(elfutils_elfutils_libasm_LIBRARY_TYPE_RELEASE STATIC)
set(elfutils_elfutils_libasm_IS_HOST_WINDOWS_RELEASE 0)
set(elfutils_elfutils_libasm_RES_DIRS_RELEASE )
set(elfutils_elfutils_libasm_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libasm_OBJECTS_RELEASE )
set(elfutils_elfutils_libasm_COMPILE_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libasm_COMPILE_OPTIONS_C_RELEASE "")
set(elfutils_elfutils_libasm_COMPILE_OPTIONS_CXX_RELEASE "")
set(elfutils_elfutils_libasm_LIBS_RELEASE asm)
set(elfutils_elfutils_libasm_SYSTEM_LIBS_RELEASE )
set(elfutils_elfutils_libasm_FRAMEWORK_DIRS_RELEASE )
set(elfutils_elfutils_libasm_FRAMEWORKS_RELEASE )
set(elfutils_elfutils_libasm_DEPENDENCIES_RELEASE elfutils::libelf elfutils::libdw)
set(elfutils_elfutils_libasm_SHARED_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libasm_EXE_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libasm_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(elfutils_elfutils_libasm_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${elfutils_elfutils_libasm_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${elfutils_elfutils_libasm_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${elfutils_elfutils_libasm_EXE_LINK_FLAGS_RELEASE}>
)
set(elfutils_elfutils_libasm_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${elfutils_elfutils_libasm_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${elfutils_elfutils_libasm_COMPILE_OPTIONS_C_RELEASE}>")
########### COMPONENT elfutils::libdw VARIABLES ############################################

set(elfutils_elfutils_libdw_INCLUDE_DIRS_RELEASE )
set(elfutils_elfutils_libdw_LIB_DIRS_RELEASE "${elfutils_PACKAGE_FOLDER_RELEASE}/lib")
set(elfutils_elfutils_libdw_BIN_DIRS_RELEASE )
set(elfutils_elfutils_libdw_LIBRARY_TYPE_RELEASE STATIC)
set(elfutils_elfutils_libdw_IS_HOST_WINDOWS_RELEASE 0)
set(elfutils_elfutils_libdw_RES_DIRS_RELEASE )
set(elfutils_elfutils_libdw_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libdw_OBJECTS_RELEASE )
set(elfutils_elfutils_libdw_COMPILE_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libdw_COMPILE_OPTIONS_C_RELEASE "")
set(elfutils_elfutils_libdw_COMPILE_OPTIONS_CXX_RELEASE "")
set(elfutils_elfutils_libdw_LIBS_RELEASE dw)
set(elfutils_elfutils_libdw_SYSTEM_LIBS_RELEASE dl)
set(elfutils_elfutils_libdw_FRAMEWORK_DIRS_RELEASE )
set(elfutils_elfutils_libdw_FRAMEWORKS_RELEASE )
set(elfutils_elfutils_libdw_DEPENDENCIES_RELEASE elfutils::libelf)
set(elfutils_elfutils_libdw_SHARED_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libdw_EXE_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libdw_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(elfutils_elfutils_libdw_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${elfutils_elfutils_libdw_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${elfutils_elfutils_libdw_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${elfutils_elfutils_libdw_EXE_LINK_FLAGS_RELEASE}>
)
set(elfutils_elfutils_libdw_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${elfutils_elfutils_libdw_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${elfutils_elfutils_libdw_COMPILE_OPTIONS_C_RELEASE}>")
########### COMPONENT elfutils::libelf VARIABLES ############################################

set(elfutils_elfutils_libelf_INCLUDE_DIRS_RELEASE )
set(elfutils_elfutils_libelf_LIB_DIRS_RELEASE "${elfutils_PACKAGE_FOLDER_RELEASE}/lib")
set(elfutils_elfutils_libelf_BIN_DIRS_RELEASE )
set(elfutils_elfutils_libelf_LIBRARY_TYPE_RELEASE STATIC)
set(elfutils_elfutils_libelf_IS_HOST_WINDOWS_RELEASE 0)
set(elfutils_elfutils_libelf_RES_DIRS_RELEASE )
set(elfutils_elfutils_libelf_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libelf_OBJECTS_RELEASE )
set(elfutils_elfutils_libelf_COMPILE_DEFINITIONS_RELEASE )
set(elfutils_elfutils_libelf_COMPILE_OPTIONS_C_RELEASE "")
set(elfutils_elfutils_libelf_COMPILE_OPTIONS_CXX_RELEASE "")
set(elfutils_elfutils_libelf_LIBS_RELEASE elf)
set(elfutils_elfutils_libelf_SYSTEM_LIBS_RELEASE pthread)
set(elfutils_elfutils_libelf_FRAMEWORK_DIRS_RELEASE )
set(elfutils_elfutils_libelf_FRAMEWORKS_RELEASE )
set(elfutils_elfutils_libelf_DEPENDENCIES_RELEASE ZLIB::ZLIB BZip2::BZip2 LibLZMA::LibLZMA zstd::libzstd_static)
set(elfutils_elfutils_libelf_SHARED_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libelf_EXE_LINK_FLAGS_RELEASE )
set(elfutils_elfutils_libelf_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(elfutils_elfutils_libelf_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${elfutils_elfutils_libelf_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${elfutils_elfutils_libelf_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${elfutils_elfutils_libelf_EXE_LINK_FLAGS_RELEASE}>
)
set(elfutils_elfutils_libelf_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${elfutils_elfutils_libelf_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${elfutils_elfutils_libelf_COMPILE_OPTIONS_C_RELEASE}>")