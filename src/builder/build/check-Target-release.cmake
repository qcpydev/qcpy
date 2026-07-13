# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libcheck_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(libcheck_FRAMEWORKS_FOUND_RELEASE "${libcheck_FRAMEWORKS_RELEASE}" "${libcheck_FRAMEWORK_DIRS_RELEASE}")

set(libcheck_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libcheck_DEPS_TARGET)
    add_library(libcheck_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libcheck_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${libcheck_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${libcheck_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libcheck_DEPS_TARGET to all of them
conan_package_library_targets("${libcheck_LIBS_RELEASE}"    # libraries
                              "${libcheck_LIB_DIRS_RELEASE}" # package_libdir
                              "${libcheck_BIN_DIRS_RELEASE}" # package_bindir
                              "${libcheck_LIBRARY_TYPE_RELEASE}"
                              "${libcheck_IS_HOST_WINDOWS_RELEASE}"
                              libcheck_DEPS_TARGET
                              libcheck_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "libcheck"    # package_name
                              "${libcheck_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libcheck_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT Check::check #############

        set(libcheck_Check_check_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(libcheck_Check_check_FRAMEWORKS_FOUND_RELEASE "${libcheck_Check_check_FRAMEWORKS_RELEASE}" "${libcheck_Check_check_FRAMEWORK_DIRS_RELEASE}")

        set(libcheck_Check_check_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET libcheck_Check_check_DEPS_TARGET)
            add_library(libcheck_Check_check_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET libcheck_Check_check_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${libcheck_Check_check_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${libcheck_Check_check_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${libcheck_Check_check_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'libcheck_Check_check_DEPS_TARGET' to all of them
        conan_package_library_targets("${libcheck_Check_check_LIBS_RELEASE}"
                              "${libcheck_Check_check_LIB_DIRS_RELEASE}"
                              "${libcheck_Check_check_BIN_DIRS_RELEASE}" # package_bindir
                              "${libcheck_Check_check_LIBRARY_TYPE_RELEASE}"
                              "${libcheck_Check_check_IS_HOST_WINDOWS_RELEASE}"
                              libcheck_Check_check_DEPS_TARGET
                              libcheck_Check_check_LIBRARIES_TARGETS
                              "_RELEASE"
                              "libcheck_Check_check"
                              "${libcheck_Check_check_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET Check::check
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${libcheck_Check_check_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${libcheck_Check_check_LIBRARIES_TARGETS}>
                     )

        if("${libcheck_Check_check_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET Check::check
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         libcheck_Check_check_DEPS_TARGET)
        endif()

        set_property(TARGET Check::check APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${libcheck_Check_check_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET Check::check APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${libcheck_Check_check_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET Check::check APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${libcheck_Check_check_LIB_DIRS_RELEASE}>)
        set_property(TARGET Check::check APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${libcheck_Check_check_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET Check::check APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${libcheck_Check_check_COMPILE_OPTIONS_RELEASE}>)


    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET Check::check APPEND PROPERTY INTERFACE_LINK_LIBRARIES Check::check)

########## For the modules (FindXXX)
set(libcheck_LIBRARIES_RELEASE Check::check)
