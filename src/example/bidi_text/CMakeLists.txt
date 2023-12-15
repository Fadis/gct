if( HARFBUZZ_FOUND AND ICUUC_FOUND )

add_executable( bidi_text
  bidi_text.cpp
)

target_link_libraries(
  bidi_text
  gct
  gct_text
  Threads::Threads
  Boost::program_options
  Boost::system
)

endif()

