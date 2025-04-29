add_executable( mesh_shader_simple
  gct.cpp
)
target_compile_definitions( mesh_shader_simple PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( mesh_shader_simple PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  mesh_shader_simple
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( mesh_shader_simple geometry/geometry.task )
add_shader( mesh_shader_simple geometry/geometry.mesh )
add_shader( mesh_shader_simple geometry/geometry.frag )
add_shader( mesh_shader_simple geometry/dummy.comp )

add_shader( mesh_shader_simple gamma/gamma.comp )

