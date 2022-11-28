set(RES_EXECUTABLE ResourceCompiler)

macro(ADD_RESOURCES OUT DIR FILENAME NAMESPACE_NAME)

    set(OUT)
    set(TEMPLATES)

    foreach (File ${ARGN})
        get_filename_component(N ${File} ABSOLUTE)
        list(APPEND TEMPLATES ${N})
    endforeach(File)

    set(OUTFILES 
        ${DIR}/${FILENAME}.h
        ${DIR}/${FILENAME}.cpp
    )

    add_custom_command(
	    OUTPUT  ${DIR}/${FILENAME}.cpp
	    COMMAND ${RES_EXECUTABLE} -o ${FILENAME} -n ${NAMESPACE_NAME} ${TEMPLATES}
	    DEPENDS ${RES_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Converting Resources"
        WORKING_DIRECTORY ${DIR}
	)

    include_directories(${DIR})
    set_source_files_properties(${OUTFILES} PROPERTIES GENERATED TRUE)

    set(${OUT} ${OUTFILES})

endmacro(ADD_RESOURCES)

