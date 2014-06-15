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
	
	IF(CMAKE_GENERATOR MATCHES Visual*)
    
		GET_TARGET_PROPERTY(oldProps ${_targetName} COMPILE_FLAGS)
		IF (${oldProps} MATCHES NOTFOUND)
			SET(oldProps "")
		ENDIF(${oldProps} MATCHES NOTFOUND)

		SET(newProperties "${oldProps} /Yu\"${_inputh}\"")
		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES COMPILE_FLAGS "${newProperties}")
		
		#also inlude ${oldProps} to have the same compile options 
		SET_SOURCE_FILES_PROPERTIES(${_path}${_inputcpp} PROPERTIES COMPILE_FLAGS "${oldProps} /Yc\"${_inputh}\"")
		
	ELSEIF (CMAKE_GENERATOR MATCHES Xcode)
		# When buiding out of the tree, precompiled may not be located
		# Use full path instead.
		GET_FILENAME_COMPONENT(fullPath ${_path}${_inputh} ABSOLUTE)

		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${fullPath}")
		SET_TARGET_PROPERTIES(${_targetName} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")

	ELSE()
    #base on https://code.google.com/p/openzone/source/browse/cmake/PCH.cmake

    # Extract CMAKE_CXX_FLAGS and CMAKE_CXX_FLAGS_XXX for the current configuration XXX.
    STRING( TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _buildTypeFlags )
    SET( _flags "${CMAKE_CXX_FLAGS} ${${_buildTypeFlags}}" )

    # Convert string of space separated flags into a list.
    SEPARATE_ARGUMENTS( _flags )

    # Extract include directories set by include_directories command.
    GET_DIRECTORY_PROPERTY( _includes INCLUDE_DIRECTORIES )
    FOREACH( _include ${_includes} )
      LIST( APPEND _flags "-I${_include}" )
    ENDFOREACH()

    # Extract definitions set by add_definitions command.
    GET_DIRECTORY_PROPERTY( _defines COMPILE_DEFINITIONS )
    FOREACH( _define ${_defines} )
      list( APPEND _flags "-D${_define}" )
    ENDFOREACH()

    SET(_pchTarget "${_targetName}PCH")

    # Helper target that properly triggers recompilation of precompiled header.
    ADD_LIBRARY( ${_pchTarget}_trigger STATIC "${_path}${_inputh}" "${_path}${_inputcpp}" )

    SET( inputHeader  "${CMAKE_CURRENT_SOURCE_DIR}/${_path}${_inputh}" )
    SET( outputHeader "${CMAKE_CURRENT_BINARY_DIR}/${_inputh}" )

    # Build precompiled header and copy original header to the build folder - GCC wants it there.add_custom_command(
    add_custom_command(
      OUTPUT  "${outputHeader}.gch"
      DEPENDS ${_pchTarget}_trigger
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${inputHeader}" "${outputHeader}"
      COMMAND "${CMAKE_COMMAND}" -E remove -f "${outputHeader}.gch"
      COMMAND "${CMAKE_CXX_COMPILER}" ${_flags} -o "${outputHeader}.gch" "${inputHeader}" )
    add_custom_target( ${_pchTarget} DEPENDS "${outputHeader}.gch" )
    set_target_properties( ${_pchTarget} PROPERTIES OUTPUT_NAME "${outputHeader}" )

    SET( ${_pchTarget}_outputPCH "${CMAKE_CURRENT_BINARY_DIR}/${outputName}.gch" )

    
    ADD_DEPENDENCIES( ${_targetName} ${_pchTarget} )
    GET_TARGET_PROPERTY( pch ${_pchTarget} OUTPUT_NAME )
    SET_TARGET_PROPERTIES( ${_targetName} PROPERTIES COMPILE_FLAGS "-include ${pch}" )

  ENDIF()

ENDMACRO(ADD_NATIVE_PRECOMPILED_HEADER)
