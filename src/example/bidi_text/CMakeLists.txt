add_executable( bidi_text
  bidi_text.cpp
)

target_link_libraries(
  bidi_text
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
)

