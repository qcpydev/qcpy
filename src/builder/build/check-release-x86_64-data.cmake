########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND libcheck_COMPONENT_NAMES Check::check)
list(REMOVE_DUPLICATES libcheck_COMPONENT_NAMES)
if(DEFINED libcheck_FIND_DEPENDENCY_NAMES)
  list(APPEND libcheck_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES libcheck_FIND_DEPENDENCY_NAMES)
else()
  set(libcheck_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(libcheck_PACKAGE_FOLDER_RELEASE "/home/brennanf/.conan2/p/b/libchf72436f84ba33/p")
set(libcheck_BUILD_MODULES_PATHS_RELEASE )


set(libcheck_INCLUDE_DIRS_RELEASE "${libcheck_PACKAGE_FOLDER_RELEASE}/include")
set(libcheck_RES_DIRS_RELEASE )
set(libcheck_DEFINITIONS_RELEASE )
set(libcheck_SHARED_LINK_FLAGS_RELEASE )
set(libcheck_EXE_LINK_FLAGS_RELEASE )
set(libcheck_OBJECTS_RELEASE )
set(libcheck_COMPILE_DEFINITIONS_RELEASE )
set(libcheck_COMPILE_OPTIONS_C_RELEASE )
set(libcheck_COMPILE_OPTIONS_CXX_RELEASE )
set(libcheck_LIB_DIRS_RELEASE "${libcheck_PACKAGE_FOLDER_RELEASE}/lib")
set(libcheck_BIN_DIRS_RELEASE )
set(libcheck_LIBRARY_TYPE_RELEASE STATIC)
set(libcheck_IS_HOST_WINDOWS_RELEASE 0)
set(libcheck_LIBS_RELEASE check)
set(libcheck_SYSTEM_LIBS_RELEASE m pthread rt)
set(libcheck_FRAMEWORK_DIRS_RELEASE )
set(libcheck_FRAMEWORKS_RELEASE )
set(libcheck_BUILD_DIRS_RELEASE )
set(libcheck_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(libcheck_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${libcheck_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${libcheck_COMPILE_OPTIONS_C_RELEASE}>")
set(libcheck_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libcheck_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libcheck_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libcheck_EXE_LINK_FLAGS_RELEASE}>")


set(libcheck_COMPONENTS_RELEASE Check::check)
########### COMPONENT Check::check VARIABLES ############################################

set(libcheck_Check_check_INCLUDE_DIRS_RELEASE "${libcheck_PACKAGE_FOLDER_RELEASE}/include")
set(libcheck_Check_check_LIB_DIRS_RELEASE "${libcheck_PACKAGE_FOLDER_RELEASE}/lib")
set(libcheck_Check_check_BIN_DIRS_RELEASE )
set(libcheck_Check_check_LIBRARY_TYPE_RELEASE STATIC)
set(libcheck_Check_check_IS_HOST_WINDOWS_RELEASE 0)
set(libcheck_Check_check_RES_DIRS_RELEASE )
set(libcheck_Check_check_DEFINITIONS_RELEASE )
set(libcheck_Check_check_OBJECTS_RELEASE )
set(libcheck_Check_check_COMPILE_DEFINITIONS_RELEASE )
set(libcheck_Check_check_COMPILE_OPTIONS_C_RELEASE "")
set(libcheck_Check_check_COMPILE_OPTIONS_CXX_RELEASE "")
set(libcheck_Check_check_LIBS_RELEASE check)
set(libcheck_Check_check_SYSTEM_LIBS_RELEASE m pthread rt)
set(libcheck_Check_check_FRAMEWORK_DIRS_RELEASE )
set(libcheck_Check_check_FRAMEWORKS_RELEASE )
set(libcheck_Check_check_DEPENDENCIES_RELEASE )
set(libcheck_Check_check_SHARED_LINK_FLAGS_RELEASE )
set(libcheck_Check_check_EXE_LINK_FLAGS_RELEASE )
set(libcheck_Check_check_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(libcheck_Check_check_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libcheck_Check_check_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libcheck_Check_check_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libcheck_Check_check_EXE_LINK_FLAGS_RELEASE}>
)
set(libcheck_Check_check_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${libcheck_Check_check_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${libcheck_Check_check_COMPILE_OPTIONS_C_RELEASE}>")