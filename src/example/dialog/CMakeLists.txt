add_executable( dialog
  dialog.cpp
)

target_link_libraries(
  dialog
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_executable( dialog2
  dialog2.cpp
)

target_link_libraries(
  dialog2
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( dialog shader.vert )
add_shader( dialog shader.frag )
add_shader( dialog global.frag )
