set(TargetName Editor.Tests)
include (Bootstrap)

set(Target_CONTENT "Content")
set(ABSOLUTE_TEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

qt_standard_project_setup()

set(RootDir  "${CMAKE_CURRENT_SOURCE_DIR}/../")


qt_add_resources(TargetName_RES ${RootDir}Interface/Icons/icons.qrc)

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
        ${TargetName_RES}
    )

    target_link_libraries(
        ${SubTargetName} 
        ${Jam_LIBRARY}
        Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Svg Qt6::Test Qt::Platform
        Editor.Source
    )

    set_target_properties(${SubTargetName} 
        PROPERTIES FOLDER "${TargetGroup}/QT"
        WIN32_EXECUTABLE   OFF
    )

    copy_target(${SubTargetName} ${Bootstrap_DIR})
    
    if (Jam_AUTO_RUN_TEST)
        add_custom_command(TARGET  ${SubTargetName} POST_BUILD
            COMMENT  "Running ${SubTargetName}"
            WORKING_DIRECTORY     ${Bootstrap_DIR}
            COMMAND ${Bootstrap_DIR}/${SubTargetName}
        )
    endif()

endforeach()


