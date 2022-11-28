find_package (Python COMPONENTS Interpreter)


# Uses the 'Enabled' definitions in OpenGL.py to generate the OpenGL API.
# TargetName:    Output file name.
# DestDirectory: The directory to write ${TargetName}.h and ${TargetName}.cpp  
set(DestDirectory ${CMAKE_SOURCE_DIR}/Source)

if(Python_Interpreter_FOUND)
    set(TargetName OpenGL)

    include(GroupSet)

    set_group(TargetName_SRC
        Source:*.py 
        Templates:OpenGLHeader.txt
        Templates:OpenGLSource.txt
    )

    # Set it up to manually run (build) and output 
    # the source and header files to the destination 
    # directory.
    add_custom_target(
        ${TargetName}                
        SOURCES ${TargetName_SRC}
        COMMAND Python::Interpreter 
            ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLWrap.py 
            --file   ${TargetName} 
            --base   ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY ${DestDirectory}
    )

    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
else()
    message(STATUS "Window_GL_REGENERATE: A Python interpreter was not found,"
    " which is needed to regenerate the OpenGL loader")
endif()
