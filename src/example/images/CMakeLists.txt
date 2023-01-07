add_executable( images
  images.cpp
)

target_link_libraries(
  images
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( images shader.vert )
add_shader( images shader.frag )
add_shader( images global.frag )
