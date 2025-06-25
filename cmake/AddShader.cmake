function(add_shader TARGET SHADER)
  cmake_parse_arguments(ADD_SHADER_ARGS "NOOPT;INSTALL" "" "" ${ARGN})
  set( USE_SPIRV_OPT FALSE )
  if(SPIRVOPT)
    if(ADD_SHADER_ARGS_NOOPT)
      set( USE_SPIRV_OPT FALSE )
    else()
      set( USE_SPIRV_OPT TRUE )
    endif()
  else()
    set( USE_SPIRV_OPT TRUE )
  endif()

  set( SHADER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/gct/shader" )
  if(USE_SPIRV_OPT)
    set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
    set(current-before_opt-path ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/intermediate_spirv/before_opt/${SHADER}.spv)
    set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/intermediate_spirv)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/intermediate_spirv/before_opt)
    get_filename_component(current-before_opt-dir ${current-before_opt-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-before_opt-dir})
  else()
    set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
    set(current-before_opt-path ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv)
    set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv)
  endif()

  get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
  file(MAKE_DIRECTORY ${current-output-dir})

  if( "${SHADER}" MATCHES "\.comp\.hlsl$" )
    string(REPLACE ".comp.hlsl.spv" ".comp.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".comp.hlsl.spv" ".comp.spv" current-output-path "${current-output-path}" )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T cs_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.vert\.hlsl$" )
    string(REPLACE ".vert.hlsl.spv" ".vert.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".vert.hlsl.spv" ".vert.spv" current-output-path "${current-output-path}" )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${cuurrent-shader-path} -T vs_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.tesc\.hlsl$" )
    string(REPLACE ".tesc.hlsl.spv" ".tesc.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".tesc.hlsl.spv" ".tesc.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T hs_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.tese\.hlsl$" )
    string(REPLACE ".tese.hlsl.spv" ".tese.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".tese.hlsl.spv" ".tese.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${cuurrent-shader-path} -T ds_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.geom\.hlsl$" )
    string(REPLACE ".geom.hlsl.spv" ".geom.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".geom.hlsl.spv" ".geom.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T gs_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.frag\.hlsl$" )
    string(REPLACE ".frag.hlsl.spv" ".frag.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".frag.hlsl.spv" ".frag.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T ps_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.mesh\.hlsl$" )
    string(REPLACE ".mesh.hlsl.spv" ".mesh.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".mesh.hlsl.spv" ".mesh.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T ms_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.task\.hlsl$" )
    string(REPLACE ".task.hlsl.spv" ".task.spv" current-before_opt-path "${current-before_opt-path}" )
    string(REPLACE ".task.hlsl.spv" ".task.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${DXC} -I${SHADER_INCLUDE_DIR}/hlsl -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T as_6_8 -Fo ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling HLSL ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.comp\.cpp$" )
    string(REPLACE ".comp.cpp.spv" ".comp.spv" current-before_opt-path "${current-before_opt-path}"  )
    string(REPLACE ".comp.cpp.spv" ".comp.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${VCC} -I${SHADER_INCLUDE_DIR}/cpp -std=c++20 --execution-model Compute --target spirv ${current-shader-path} -o ${current-before_opt-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling CXX Shader ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.vert\.cpp$" )
    string(REPLACE ".vert.cpp.spv" ".vert.spv" current-before_opt-path "${current-before_opt-path}"  )
    string(REPLACE ".vert.cpp.spv" ".vert.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${VCC} -I${SHADER_INCLUDE_DIR}/cpp -std=c++20 --execution-model Vertex --target spirv ${cuurrent-shader-path} -o ${current-before_opt-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling CXX Shader ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.frag\.cpp$" )
    string(REPLACE ".frag.cpp.spv" ".frag.spv" current-before_opt-path "${current-before_opt-path}"  )
    string(REPLACE ".frag.cpp.spv" ".frag.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${VCC} -I${SHADER_INCLUDE_DIR}/cpp -std=c++20 --execution-model Fragment --target spirv ${current-shader-path} -o ${current-before_opt-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling CXX Shader ${SHADER} ..."
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.cl$" )
    string(REPLACE ".cl.spv" ".comp.spv" current-before_opt-path "${current-before_opt-path}"  )
    string(REPLACE ".cl.spv" ".comp.spv" current-output-path "${current-output-path}" )
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${CLSPV} -I${SHADER_INCLUDE_DIR}/cl --cl-std=CLC++2021 -inline-entry-points --spv-version=1.6 ${current-shader-path} -o ${current-before_opt-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling OpenCL ${SHADER} ..."
      VERBATIM)
  else()
    add_custom_command(
      OUTPUT ${current-before_opt-path}
      COMMAND ${GLSLC} -I${SHADER_INCLUDE_DIR}/glsl -o ${current-before_opt-path} ${current-shader-path} --target-env=vulkan1.3 -w -Werror
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Compiling GLSL ${SHADER} ..."
      VERBATIM)
  endif()

  if(USE_SPIRV_OPT)
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND spirv-opt -o ${current-output-path} ${current-before_opt-path} --target-env=vulkan1.3 -O
      DEPENDS ${current-before_opt-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      COMMENT "Optimizing ${SHADER} ..."
      VERBATIM)
    set_source_files_properties(${current-before_opt-path} PROPERTIES GENERATED TRUE)
  endif()
  set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
  target_sources(${TARGET} PRIVATE ${current-output-path})
  if(ADD_SHADER_ARGS_INSTALL)
    cmake_path(REMOVE_FILENAME SHADER OUTPUT_VARIABLE SHADER_DIR)
    install(
      FILES ${current-output-path}
      DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/${SHADER_DIR}
      PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
    )
  endif()
endfunction(add_shader)

