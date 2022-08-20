add_executable( roughness_mask
  roughness_mask.cpp
)

target_link_libraries(
  roughness_mask
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

add_shader( roughness_mask shader.vert )
add_shader( roughness_mask shader.frag )

