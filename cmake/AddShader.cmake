function(add_shader TARGET SHADER)

  set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER})
  set(current-output-path ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv)

  get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
  file(MAKE_DIRECTORY ${current-output-dir})
  if( "${SHADER}" MATCHES "\.comp\.hlsl$" )
    string(REPLACE ".comp.hlsl.spv" ".comp.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T cs_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.vert\.hlsl$" )
    string(REPLACE ".vert.hlsl.spv" ".vert.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${cuurrent-shader-path} -T vs_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.tesc\.hlsl$" )
    string(REPLACE ".tesc.hlsl.spv" ".tesc.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T hs_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.tese\.hlsl$" )
    string(REPLACE ".tese.hlsl.spv" ".tese.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${cuurrent-shader-path} -T ds_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.geom\.hlsl$" )
    string(REPLACE ".geom.hlsl.spv" ".geom.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T gs_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.frag\.hlsl$" )
    string(REPLACE ".frag.hlsl.spv" ".frag.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T ps_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.mesh\.hlsl$" )
    string(REPLACE ".mesh.hlsl.spv" ".mesh.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T ms_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.task\.hlsl$" )
    string(REPLACE ".task.hlsl.spv" ".task.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${DXC} -spirv -fspv-target-env=vulkan1.3 ${current-shader-path} -T as_6_8 -Fo ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.comp\.cpp$" )
    string(REPLACE ".comp.cpp.spv" ".comp.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${VCC} -std=c++20 --execution-model Compute --target spirv ${current-shader-path} -o ${current-output-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.vert\.cpp$" )
    string(REPLACE ".vert.cpp.spv" ".vert.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${VCC} -std=c++20 --execution-model Vertex --target spirv ${cuurrent-shader-path} -o ${current-output-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.frag\.cpp$" )
    string(REPLACE ".frag.cpp.spv" ".frag.spv" current-output-path "${current-output-path}"  )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${VCC} -std=c++20 --execution-model Fragment --target spirv ${current-shader-path} -o ${current-output-path} --entry-point main
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  elseif( "${SHADER}" MATCHES "\.cl$" )
    string(REPLACE ".cl.spv" ".comp.spv" current-output-path "${current-output-path}" )
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${CLSPV} --cl-std=CLC++2021 -inline-entry-points --spv-version=1.6 ${current-shader-path} -o ${current-output-path}
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  else()
    add_custom_command(
      OUTPUT ${current-output-path}
      COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path} --target-env=vulkan1.3
      DEPENDS ${current-shader-path}
      IMPLICIT_DEPENDS CXX ${current-shader-path}
      VERBATIM)
  endif()

  set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
  target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)

