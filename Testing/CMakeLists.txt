set(TargetName Engine.Tests)
include (GroupSet)


file(GLOB TestTarget_SRC *.cpp)

set_group(Scan_SRC 
    Scan:*.eq
    TestData:*.inl
)

set(ABSOLUTE_TEST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
 ${CMAKE_CURRENT_SOURCE_DIR}/TestDirectory.h.in 
 ${CMAKE_CURRENT_BINARY_DIR}/TestDirectory.h
)

include_directories(. 
	${GTEST_INCLUDE} 
        ${CMAKE_CURRENT_BINARY_DIR} 
        ${Jam_INCLUDE})

add_executable(
    ${TargetName}
    ${TestTarget_SRC}
    ${Bootstrap_SRC}
    ${Scan_SRC}
    ${CMAKE_CURRENT_BINARY_DIR}/TestDirectory.h
)

target_link_libraries(${TargetName} 
    ${GTEST_LIBRARY} 
    ${Jam_LIBRARY}
)

set_target_properties(${TargetName} 
    PROPERTIES FOLDER "${TargetGroup}")


if (Jam_AUTO_RUN_TEST)
    add_custom_command(TARGET  ${TargetName} POST_BUILD
        WORKING_DIRECTORY     ${Bootstrap_DIR}
        COMMAND               $<TARGET_FILE:${TargetName}>
    )
        
endif()


set_target_properties(${TargetName} PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}
)
