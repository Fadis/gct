if( XKBCOMMON_FOUND AND SDBUSCPP_FOUND AND LIBINPUT_FOUND )

add_executable( text_input
  text_input.cpp
)

target_link_libraries(
  text_input
  gct
  gct_input
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_shader( text_input shader.vert )
add_shader( text_input global.frag )

add_executable( fcitx5_input
  fcitx5_input.cpp
)

target_link_libraries(
  fcitx5_input
  gct
  gct_input
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

endif()

