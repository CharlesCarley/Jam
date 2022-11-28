set(TargetName Equation)
include(GroupSet)


set_group(Target_HDR
    Header:*.h
    Header:*.grm
    Header:*.inl
    )
set_group(Target_SRC
    Source:*.cpp
    )



include_directories(. ../ ${Utils_INCLUDE} ${Math_INCLUDE})
add_library(${TargetName} ${Target_SRC} ${Target_HDR})

target_link_libraries(${TargetName} ${Utils_LIBRARY} ${Math_LIBRARY})


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
