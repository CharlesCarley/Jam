cmake_minimum_required(VERSION  3.15)
project(Jam)

# Use C++-17 by default.
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(TargetFolders TRUE)
set(TargetName )
set(TargetGroup )


# Set the module path to include scripts
# CMake - Contains project local modules.
#
# CMake/Modules - Contains project independent modules.
#   GitUpdate.cmake invokes python ${Jam_SOURCE_DIR}/gitupdate.py
#   to pull all submodules and update to head.
#   If this fails, it will attempt to invoke git directly to pull 
#   submodules. 
list(APPEND CMAKE_MODULE_PATH
        ${Jam_SOURCE_DIR}/Config
        ${Jam_SOURCE_DIR}/Config/Modules
        ${Jam_SOURCE_DIR}/Extras/BuildTools/ResourceCompiler/CMake
        )

include(Configure)
if (NOT Configure_SUCCEEDED)
    return()
endif ()

list(APPEND CMAKE_MODULE_PATH
        ${Jam_QT_ROOT}
        )

add_definitions(${Jam_GLOBAL_DEFINE})

# Bring in the build utility scripts
set(TargetGroup "CMakePredefinedTargets")
add_subdirectory(Config)

#set(TargetGroup "Extras/BuildTools")
#add_subdirectory(Extras/OpenGL)


#set(TargetFolders TRUE)
#set(TargetGroup "Libraries/ThirdParty")
#set(SDL_TargetGroup ${TargetGroup})
#set(FreeImage_TargetGroup ${TargetGroup})
#add_subdirectory(Libraries/ThirdParty/SDL)
#add_subdirectory(Libraries/ThirdParty/FreeImage)

set(TargetGroup "Libraries/Intern")
add_subdirectory(Libraries/Intern/Math)
add_subdirectory(Libraries/Intern/Utils)
add_subdirectory(Libraries/Intern/Equation)
add_subdirectory(Libraries/Intern/Xml)


set(TargetGroup "Application")
add_subdirectory(Source)

set(TargetGroup "Extras/BuildTools")
add_subdirectory(Extras/BuildTools/ResourceCompiler)


# Setup for testing
if (Jam_BUILD_TEST)
    set(TargetGroup "Extra/")

    if (NOT Jam_USE_STATIC_RUNTIME)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()

    add_subdirectory(${GTEST_DIR})

    set(TargetGroup "Libraries/ThirdParty")
    if (TargetFolders)
        set_target_properties(gtest_main PROPERTIES FOLDER "${TargetGroup}")
        set_target_properties(gtest PROPERTIES FOLDER "${TargetGroup}")
    endif ()

    set(TargetGroup "Application/Tests")
    add_subdirectory(Testing)

    if (Jam_BUILD_EDITOR)
        add_subdirectory(Source/Tests)
    endif()

endif ()

