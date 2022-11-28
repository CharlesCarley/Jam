
if (NOT MSVC)
    return()
endif()

include(GroupSet)
set(TargetName BuildConfig)
set(GroupRoot ../)

set_group(TargetName_SRC
    :${GroupRoot}/.clang-format
    :${GroupRoot}/CMakeLists.txt
    Config:${GroupRoot}/Config/CMakeLists.txt
    Config:${GroupRoot}/Config/*.cmake
    Config/Modules:${GroupRoot}/Config/Modules/*.cmake
)

set_source_files_properties(${TargetName_SRC} PROPERTIES VS_TOOL_OVERRIDE "None")

add_custom_target(
    ${TargetName} 
    ALL SOURCES ${TargetName_SRC}
    )


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
