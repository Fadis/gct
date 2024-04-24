add_executable( onesweep
  gct.cpp
)
target_compile_definitions( onesweep PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( onesweep PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  onesweep
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( onesweep onesweep32/histgram.comp )
add_shader( onesweep onesweep32/histgram_sum.comp )
add_shader( onesweep onesweep32/local_sum.comp )
add_shader( onesweep onesweep32/global_sum.comp )
add_shader( onesweep onesweep32/sort.comp )
add_shader( onesweep onesweep32/small_sort.comp )

