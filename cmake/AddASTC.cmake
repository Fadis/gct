function(add_astc TARGET SHADER SRGB)

	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.astc)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
  if( SRGB )
    set( astc-color-mode "-cs" )
  else()
    set( astc-color-mode "-cl" )
  endif()
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${ASTCENC} ${astc-color-mode} ${current-input-path} ${current-output-path} 8x8 100
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_astc)

