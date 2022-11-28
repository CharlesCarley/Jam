set(TargetName Xml)
include(GroupSet)


set_group(Target_HDR
    Header:*.h
    Reference:*.grm
    )
set_group(Target_SRC
    Source:*.cpp
    )



include_directories(. ../ ${Utils_INCLUDE})
add_library(${TargetName} ${Target_SRC} ${Target_HDR})

target_link_libraries(${TargetName} ${Utils_LIBRARY})


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
