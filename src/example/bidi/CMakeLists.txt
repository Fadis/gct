add_executable( bidi
  bidi.cpp
)

target_link_libraries(
  bidi
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( bidi shader.vert )
add_shader( bidi shader.frag )
add_shader( bidi global.frag )
