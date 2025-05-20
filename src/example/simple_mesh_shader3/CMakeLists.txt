add_executable( simple_mesh_shader3
  gct.cpp
)
target_compile_definitions( simple_mesh_shader3 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( simple_mesh_shader3 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  simple_mesh_shader3
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

add_shader( simple_mesh_shader3 geometry/geometry.mesh )
add_shader( simple_mesh_shader3 geometry/geometry.frag )
add_shader( simple_mesh_shader3 geometry/dummy.comp )
add_shader( simple_mesh_shader3 gamma/gamma.comp )

