add_executable( dump_texture
  gct.cpp
)

target_link_libraries(
  dump_texture
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

add_shader( dump_texture shader.vert )
add_shader( dump_texture shader.frag )

