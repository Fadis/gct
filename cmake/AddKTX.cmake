function(add_bc1_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc1.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=bc1  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc1_ktx)

function(add_bc3_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc3.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=bc3  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc3_ktx)

function(add_bc4_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc4.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=bc4  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc4_ktx)

function(add_bc5_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc5.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=bc5  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc5_ktx)

function(add_bc6h_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.bc6h.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=bc6h  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_bc6h_ktx)

function(add_etc1_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.etc1.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=etc1  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_etc1_ktx)

function(add_etc2_rgb_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.etc2_rgb.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=etc2_rgb  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_etc2_rgb_ktx)

function(add_etc2_rgba_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.etc2_rgba.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=etc2_rgba  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_etc2_rgba_ktx)

function(add_eac_r11_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.eac_r11.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=eac_r11  --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_eac_r11_ktx)

function(add_eac_rg11_ktx TARGET INPUT_FILENAME)
	set(current-input-path ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILENAME})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.eac_rg11.ktx)

	get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
	file(MAKE_DIRECTORY ${current-output-dir})
	add_custom_command(
		OUTPUT ${current-output-path}
    COMMAND ${BETSY} --codec=eac_rg11 --quality=2 ${current-input-path} ${current-output-path}
		DEPENDS ${current-input-path}
		IMPLICIT_DEPENDS CXX ${current-input-path}
		VERBATIM)

	set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
	target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_eac_rg11_ktx)

