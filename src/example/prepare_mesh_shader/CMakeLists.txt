add_executable( prepare_mesh_shader
  vulkan_hpp.cpp
)
target_compile_definitions( prepare_mesh_shader PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( prepare_mesh_shader PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  prepare_mesh_shader
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

add_shader( prepare_mesh_shader geometry/geometry.mesh )
add_shader( prepare_mesh_shader geometry/geometry.frag )

