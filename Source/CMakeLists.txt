include(ResourceCompiler)
include(CopyTargets)
include(VsDependentUpon)
include(Bootstrap)
qt_standard_project_setup()



set(TargetNameLib Editor.Source)
set(TargetName    Editor)

set(ContentFolder "Content")
set(SourceFolder  "Source Files")
set(HeaderFolder  "Header Files")


set_property(GLOBAL PROPERTY  
             AUTOGEN_SOURCE_GROUP  
             "${ContentFolder}/Generated")

set_group(TargetName_SRC
  ${SourceFolder}/Utils:Utils/*.cpp

  ${SourceFolder}/Interface:Interface/*.cpp
  ${SourceFolder}/Interface:Interface/*.inl
  ${SourceFolder}/Interface/View:Interface/View/*.cpp
  
  ${SourceFolder}/Interface/Area:Interface/Area/*.cpp
  ${SourceFolder}/Interface/Area:Interface/Area/*.inl

  ${SourceFolder}/Interface/Dialogs:Interface/Dialogs/*.cpp

  ${SourceFolder}/Interface/Widgets:Interface/Widgets/*.cpp

  ${SourceFolder}/Interface/Areas:Interface/Areas/*.cpp
  ${SourceFolder}/Interface/Style:Interface/Style/*.cpp

  ${SourceFolder}/State:State/*.cpp
  ${SourceFolder}/State/FrameStack:State/FrameStack/*.cpp
  ${SourceFolder}/State/IO:State/IO/*.cpp
 

  ${HeaderFolder}/Utils:Utils/*.h
  ${HeaderFolder}/Interface:Interface/*.h
  ${HeaderFolder}/Interface/View:Interface/View/*.h

  ${HeaderFolder}/Interface/Events:Interface/Events/*.h

  ${HeaderFolder}/Interface/Areas:Interface/Areas/*.h
  ${HeaderFolder}/Interface/Area:Interface/Area/*.h

  ${HeaderFolder}/Interface/Style:Interface/Style/*.h

  ${HeaderFolder}/Interface/Dialogs:Interface/Dialogs/*.h
  ${HeaderFolder}/Interface/Widgets:Interface/Widgets/*.h

  ${HeaderFolder}/State:State/*.h
  ${HeaderFolder}/State/FrameStack:State/FrameStack/*.h
  ${HeaderFolder}/State/IO:State/IO/*.h

  ${ContentFolder}/Extras/Icons:Interface/Icons/*.svg
  ${ContentFolder}/Extras/Icons:Interface/*.qrc
)

vs_dependent_upon(Interface/Application.cpp
    Interface/Application.Run.inl
    Interface/Application.Dialogs.inl
    Interface/Application.Menu.inl
    Interface/Application.Project.inl
    Interface/Application.Events.inl
    Interface/Application.Settings.inl
)

vs_dependent_upon(Interface/Area/AreaNode.cpp
    Interface/Area/AreaNode.Accessors.inl
    Interface/Area/AreaNode.Events.inl
    Interface/Area/AreaNode.Utilities.inl
)


vs_dependent_upon(Interface/MainArea.cpp
    Interface/MainArea.Creator.inl
)
include_directories(. ${Jam_INCLUDE})

add_library(${TargetNameLib} ${TargetName_SRC})
set_target_properties(${TargetNameLib} 
    PROPERTIES FOLDER "${TargetGroup}")

target_link_libraries(${TargetNameLib} 
    Qt6::Widgets 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Svg 
    ${Jam_LIBRARY})

# -----------------------------------------------------------------------------
qt_add_resources(TargetName_RES Interface/Icons/icons.qrc)

source_group("${ContentFolder}" FILES  ${Jam_IconSource} )
source_group("${ContentFolder}/Generated" FILES  ${TargetName_RES} )


add_executable(${TargetName} 
    Editor.cpp 
    Editor.h
    ${TargetName_RES}
    ${Jam_IconSource}
)

target_link_libraries(${TargetName} 
    ${TargetNameLib} 
    ${Jam_LIBRARY})

set_target_properties(${TargetName} PROPERTIES
    WIN32_EXECUTABLE               OFF
    MACOSX_BUNDLE                  OFF
    FOLDER                         "${TargetGroup}"
    VS_DPI_AWARE                   "PerMonitor"
)

copy_target(${TargetName} ${Bootstrap_DIR})
