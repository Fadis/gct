if( XKBCOMMON_FOUND AND SDBUSCPP_FOUND AND LIBINPUT_FOUND )

add_executable( libinput_keycode
  libinput_keycode.cpp
)

target_link_libraries(
  libinput_keycode
  gct
  gct_input
  Threads::Threads
  Boost::program_options
  Boost::system
)

add_executable( libinput_keysym
  libinput_keysym.cpp
)

target_link_libraries(
  libinput_keysym
  gct
  gct_input
  Threads::Threads
  Boost::program_options
  Boost::system
)

endif()

