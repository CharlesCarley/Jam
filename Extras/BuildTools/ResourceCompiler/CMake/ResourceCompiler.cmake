set(RES_EXECUTABLE ResourceCompiler)

macro(ADD_RESOURCES OUT DIR FILENAME NS CN)

    set(OUT)
    set(TEMPLATES)
    
    foreach (File ${ARGN})
        get_filename_component(N ${File} ABSOLUTE)
        list(APPEND TEMPLATES "${N}")
    endforeach(File)

    set(OUTFILES 
        ${DIR}/${FILENAME}.h
        ${DIR}/${FILENAME}.cpp
    )

    add_custom_command(
	    OUTPUT  ${DIR}/${FILENAME}.cpp
	    COMMAND ${RES_EXECUTABLE} -o "${FILENAME}" -c "${CN}" -n "${NS}" ${TEMPLATES}
	    DEPENDS ${RES_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Converting Resources"
        WORKING_DIRECTORY ${DIR}
	)

    include_directories(${DIR})

    foreach (File ${TEMPLATES})
        list(APPEND OUTFILES ${File})
    endforeach(File)
    set_source_files_properties(${OUTFILES} PROPERTIES GENERATED TRUE)


    set(${OUT} ${OUTFILES})

endmacro(ADD_RESOURCES)



