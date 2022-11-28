set(TargetName Math)
include (GroupSet)

set_group(Target_HDR
    Header:*.h
    )
set_group(Target_SRC
    Source:*.cpp
    )


include_directories(../)

add_library(${TargetName}
            ${Target_SRC} ${Target_HDR}
            )

target_link_libraries(${TargetName})


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
