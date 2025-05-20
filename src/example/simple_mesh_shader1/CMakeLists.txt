add_executable( simple_mesh_shader1
  gct.cpp
)
target_compile_definitions( simple_mesh_shader1 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( simple_mesh_shader1 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  simple_mesh_shader1
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

add_shader( simple_mesh_shader1 geometry/geometry.mesh )
add_shader( simple_mesh_shader1 geometry/geometry.frag )
add_shader( simple_mesh_shader1 geometry/dummy.comp )
add_shader( simple_mesh_shader1 gamma/gamma.comp )

