add_executable( text
  text.cpp
)

target_link_libraries(
  text
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( text shader.vert )
add_shader( text shader.frag )
add_shader( text global.frag )
