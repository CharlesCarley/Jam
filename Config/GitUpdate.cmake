
set(TestFile ${GTEST_DIR}/CMakeLists.txt)
set(GitUpdate_SUCCESS FALSE)

# Attempts to pull submodules with python or git.
find_package(Python COMPONENTS Interpreter)

if(Python_Interpreter_FOUND)

	if (NOT EXISTS ${TestFile})
		execute_process(COMMAND ${Python_EXECUTABLE} 
				${CMAKE_SOURCE_DIR}/gitupdate.py
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		)
	endif()


	if (EXISTS ${TestFile})
		set(GitUpdate_SUCCESS TRUE)
	endif()

else()

	find_package(Git)

	if (GIT_FOUND)
		if (NOT EXISTS ${TestFile})
			execute_process(COMMAND ${GIT_EXECUTABLE} 
					submodule update --init
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			)
		endif()
		if (EXISTS ${TestFile})
			set(GitUpdate_SUCCESS TRUE)
		endif()
	endif()
endif()


if (NOT GitUpdate_SUCCESS)
	message("")
	message("GitUpdate: Was unable to automatically pull required submodules.")
	message("Please manually invoke: ")
	message("git submodule update --init")
endif()
