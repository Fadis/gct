add_executable( diffuse
  diffuse.cpp
)

target_link_libraries(
  diffuse
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

add_shader( diffuse shader.vert )
add_shader( diffuse shader.frag )
