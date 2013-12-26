MACRO(_PCH_GET_COMPILE_FLAGS _out_compile_flags)

  STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
  SET(${_out_compile_flags} ${${_flags_var_name}} )
  
  IF(CMAKE_COMPILER_IS_GNUCXX)
    
    GET_TARGET_PROPERTY(_targetType ${_PCH_current_target} TYPE)
    IF(${_targetType} STREQUAL SHARED_LIBRARY)
      LIST(APPEND ${_out_compile_flags} "${${_out_compile_flags}} -fPIC")
    ENDIF(${_targetType} STREQUAL SHARED_LIBRARY)
    
  ELSE(CMAKE_COMPILER_IS_GNUCXX)
    ## TODO ... ? or does it work out of the box 
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  
  GET_DIRECTORY_PROPERTY(DIRINC INCLUDE_DIRECTORIES )
  FOREACH(item ${DIRINC})
    LIST(APPEND ${_out_compile_flags} "-I${item}")
  ENDFOREACH(item)
  
  GET_DIRECTORY_PROPERTY(_directory_flags DEFINITIONS)
  #MESSAGE("_directory_flags ${_directory_flags}" )
  LIST(APPEND ${_out_compile_flags} ${_directory_flags})
  LIST(APPEND ${_out_compile_flags} ${CMAKE_CXX_FLAGS} )
  
  SEPARATE_ARGUMENTS(${_out_compile_flags})

ENDMACRO(_PCH_GET_COMPILE_FLAGS)

MACRO(ADD_NATIVE_PRECOMPILED_HEADER _targetName _path _inputh _inputcpp)

	IF( "${ARGN}" STREQUAL "0")
		SET(_dowarn 0)
	ELSE( "${ARGN}" STREQUAL "0")
		SET(_dowarn 1)
	ENDIF("${ARGN}" STREQUAL "0")
	
	if(CMAKE_GENERATOR MATCHES Visual*)
    
		GET_TARGET_PROPERTY(oldProps ${_targetName} COMPILE_FLAGS)
		if (${oldProps} MATCHES NOTFOUND)
			SET(oldProps "")
		endif(${oldProps} MATCHES NOTFOUND)

		SET(newProperties "${oldProps} /Yu\"${_inputh}\"")
		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES COMPILE_FLAGS "${newProperties}")
		
		#also inlude ${oldProps} to have the same compile options 
		SET_SOURCE_FILES_PROPERTIES(${_path}${_inputcpp} PROPERTIES COMPILE_FLAGS "${oldProps} /Yc\"${_inputh}\"")
		
	elseif (CMAKE_GENERATOR MATCHES Xcode)
		# When buiding out of the tree, precompiled may not be located
		# Use full path instead.
		GET_FILENAME_COMPONENT(fullPath ${_path}${_inputh} ABSOLUTE)

		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${fullPath}")
		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")

	else()
        GET_TARGET_PROPERTY(oldProps ${_targetName} COMPILE_FLAGS)
        if (${oldProps} MATCHES NOTFOUND)
          SET(oldProps "")
        endif(${oldProps} MATCHES NOTFOUND)
        SET(newProperties "${oldProps} -include ${_inputh}")
        SET_TARGET_PROPERTIES(${_targetName} PROPERTIES COMPILE_FLAGS "${newProperties}")

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
          SET(pchOutput "${_inputh}.pch")
        else()
          SET(pchOutput "${_inputh}.gch")
        endif()
        GET_FILENAME_COMPONENT(fullPath ${_path}${_inputh} ABSOLUTE)
        # add pch build target
        _PCH_GET_COMPILE_FLAGS(_compile_FLAGS)
        #MESSAGE(${_compile_FLAGS})
        SET(_pchTarget ${_targetName}PCH)
        add_custom_command( OUTPUT ${pchOutput}
          DEPENDS ${fullPath}
          COMMAND "${CMAKE_CXX_COMPILER}" ${CMAKE_CXX_COMPILER_ARG1} -c -isysroot ${CMAKE_OSX_SYSROOT} ${_compile_FLAGS} -F/Library/Frameworks -o ${pchOutput} -x c++-header "${fullPath}" )
        add_custom_target( ${_pchTarget} DEPENDS ${pchOutput} )
        add_dependencies( ${_targetName} ${_pchTarget} )
        
	endif()

ENDMACRO(ADD_NATIVE_PRECOMPILED_HEADER)
