set(GTEST_DIR ${Jam_SOURCE_DIR}/Libraries/ThirdParty/googletest)
include(GitUpdate)
include(GroupSet)


if (NOT GitUpdate_SUCCESS)
    return()
endif ()

include(StaticRuntime)
include(ExternalTarget)
include(GroupSet)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Jam_BUILD_TEST "Build the unit test program." ON)
option(Jam_AUTO_RUN_TEST "Automatically run the test program." OFF)
option(Jam_Window_GL_REGENERATE "Regenerate the OpenGL API from the Extras/OpenGL.py dictionary." OFF)
option(Jam_JUST_MY_CODE "Enable the /JMC flag" ON)
option(Jam_USE_STATIC_RUNTIME  "Build with the MultiThreaded(Debug) runtime library." ON)
option(Jam_BUILD_EDITOR  "Build the Qt editor." ON)


set(Jam_BIN_DIR ${CMAKE_SOURCE_DIR}/Bin CACHE PATH "")
include(Bootstrap)


if (${Jam_BUILD_EDITOR})
    # This must be forced to off, qt requires that the runtime 
    # be MT(_d)Dll. 
    set(Jam_USE_STATIC_RUNTIME OFF CACHE BOOL "" FORCE)
endif()


if (${Jam_USE_STATIC_RUNTIME})
    set_static_runtime()
else()
    set_dynamic_runtime()
endif()

set(Jam_GLOBAL_DEFINE -DSDL_MAIN_HANDLED)

# Global icon source
set(Jam_IconSource 
   ${CMAKE_SOURCE_DIR}/Extras/AppIcon/Win32/Resource.rc
   ${CMAKE_SOURCE_DIR}/Extras/AppIcon/Win32/AppIcon.ico
 )


# Bring in google test
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(GTEST_INCLUDE ${GTEST_DIR}/include)
set(GTEST_LIBRARY gtest_main)

# Include opengl
if (NOT USING_EMSCRIPTEN)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL QUIET REQUIRED)
    set(OpenGL_LIBRARY ${OPENGL_LIBRARIES})
endif ()

# Set up for SDL
if (NOT USING_EMSCRIPTEN)
    DefineExternalTarget(SDL "ThirdParty" ${Jam_SOURCE_DIR}/Libraries/ThirdParty/SDL/SDL/include)
    set(SDL_LIBRARY SDL2-static ${OpenGL_LIBRARY})
    set(SDL_MAIN_LIB SDL2main)
    set(SDL_FOLDER Extern)
    set(SDL_TargetShared OFF)
endif()

# Qt setup
set(Jam_QT_HOME $ENV{Qt6} CACHE STRING  "")
set(Jam_QT_VERSION "6.4.0" CACHE STRING  "")
set(Jam_QT_BUILD "msvc2019_64" CACHE STRING  "")
set(Jam_QT_ROOT  "${Jam_QT_HOME}\\${Jam_QT_VERSION}\\${Jam_QT_BUILD}" CACHE STRING "" FORCE)

set(CMAKE_PREFIX_PATH ${Jam_QT_ROOT})
find_package(Qt6 COMPONENTS Core Widgets Gui Svg Test)

set(Utils_INCLUDE ${Jam_SOURCE_DIR}/Libraries/Intern)
set(Utils_LIBRARY Utils)

set(Xml_INCLUDE ${Jam_SOURCE_DIR}/Libraries/Intern)
set(Xml_LIBRARY Xml)

set(Math_INCLUDE ${Jam_SOURCE_DIR}/Libraries/Intern)
set(Math_LIBRARY Math)

set(Eq_INCLUDE ${Jam_SOURCE_DIR}/Libraries/Intern)
set(Eq_LIBRARY Equation)

set(Jam_INCLUDE 
        ${Jam_SOURCE_DIR}/
        ${Utils_INCLUDE}
        ${Xml_INCLUDE}
        ${Math_INCLUDE}
        ${Eq_INCLUDE}
)

set(Jam_LIBRARY  
        ${Utils_LIBRARY}
        ${Xml_LIBRARY}
        ${Math_LIBRARY}
        ${Eq_LIBRARY})

set(ExtraFlags )
if (MSVC)
    # globally disable scoped enum warnings
    set(ExtraFlags "${ExtraFlags} /wd26812")


    if (Jam_JUST_MY_CODE)
        # Enable just my code...
        set(ExtraFlags "${ExtraFlags} /JMC")
    endif ()
    set(ExtraFlags "${ExtraFlags} /fp:precise")
    set(ExtraFlags "${ExtraFlags} /fp:except")
    set(ExtraFlags "${ExtraFlags} /openmp")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")
else ()
    set(ExtraFlags "${ExtraFlags} -Os")
    set(ExtraFlags "${ExtraFlags} -O3")
    set(ExtraFlags "${ExtraFlags} -fPIC")
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} stdc++fs")
    set(Jam_EXTRA_LIBS "stdc++fs")
endif ()

message(STATUS "Extra global flags: ${ExtraFlags}")
message(STATUS "Global flags: ${CMAKE_CXX_FLAGS}")

# Flag that the configure did not succeed
set(Configure_SUCCEEDED TRUE)