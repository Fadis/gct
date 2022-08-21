add_executable( astc
  astc.cpp
)

target_link_libraries(
  astc
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

add_shader( astc shader.vert )
add_shader( astc shader.frag )
add_astc( astc globe_color.png TRUE )
add_astc( astc globe_normal.png FALSE )
add_astc( astc globe_roughness.png FALSE )

