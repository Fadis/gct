add_executable( font
  font.cpp
)

target_link_libraries(
  font
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( font shader.vert )
add_shader( font shader.frag )

