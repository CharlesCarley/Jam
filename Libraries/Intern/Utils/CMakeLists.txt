set(TargetName Utils)
include (GroupSet)

set_group(Utils_HDR
    Header:*.h
    Header/CommandLine:CommandLine/*.h
    Header/ParserBase:ParserBase/*.h
    Header/LeakCheck:Win32/*.h
    )
set_group(Utils_SRC
    Source:*.hint
    Source:*.cpp
    Source/CommandLine:CommandLine/*.cpp
    Source/ParserBase:ParserBase/*.cpp
    Source/LeakCheck:Win32/*.cpp
    )


include_directories(../)

add_library(${TargetName}
            ${Utils_SRC} ${Utils_HDR}
            )

target_link_libraries(${TargetName})


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
