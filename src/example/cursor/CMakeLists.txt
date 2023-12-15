if( XKBCOMMON_FOUND AND SDBUSCPP_FOUND AND LIBINPUT_FOUND )

add_executable( cursor
  cursor.cpp
)

target_link_libraries(
  cursor
  gct
  gct_input
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( cursor shader.vert )
add_shader( cursor global.frag )

endif()

