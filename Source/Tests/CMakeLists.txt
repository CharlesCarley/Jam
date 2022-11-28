set(TargetName Editor.Tests)
include (Bootstrap)

set(Target_CONTENT "Content")
set(ABSOLUTE_TEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(TestConfig     "TestDirectory.h")
set(TestConfig_SRC "${CMAKE_CURRENT_BINARY_DIR}/${TestConfig}")

qt_standard_project_setup()



configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/${TestConfig}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${TestConfig}"
)

set(RootDir  "${CMAKE_CURRENT_SOURCE_DIR}/../")



file(GLOB TestTarget_SRC *.cpp)

foreach (file ${TestTarget_SRC})
    get_filename_component(Target_BASE ${file} NAME_WE)

    set(SubTargetName Test.${Target_BASE})
    
    message(STATUS "QT Test = ${SubTargetName}")

    include_directories(
         ${CMAKE_CURRENT_SOURCE_DIR} 
         ${CMAKE_CURRENT_BINARY_DIR} 
         ${Jam_INCLUDE}
         ${RootDir}
        )

    add_executable(${SubTargetName} 
        "${Target_BASE}.cpp" 
        "${Target_BASE}.h"
        ${TestConfig_SRC}
    )



    target_link_libraries(
        ${SubTargetName} 
        ${Jam_LIBRARY}
        Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Svg  Qt6::Test
        Editor.Source
    )

    set_target_properties(${SubTargetName} 
        PROPERTIES FOLDER "${TargetGroup}/QT"
    )
    
endforeach()


