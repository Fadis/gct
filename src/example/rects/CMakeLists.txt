add_executable( rects
  rects.cpp
)

target_link_libraries(
  rects
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( rects shader.vert )
add_shader( rects shader.frag )
add_shader( rects global.frag )
