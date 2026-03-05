function(add_bc7 TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc7.dds)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${TODDS} --clean --format BC7 --overwrite ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc7)

function(add_bc1 TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc1.dds)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${TODDS} --clean --format BC1 --overwrite ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc1)

