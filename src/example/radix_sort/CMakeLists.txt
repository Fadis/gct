add_executable( radix_sort
  gct.cpp
)
target_compile_definitions( radix_sort PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( radix_sort PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  radix_sort
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

add_shader( radix_sort radix_sort32/local_sum.comp )
add_shader( radix_sort radix_sort32/global_sum.comp )
add_shader( radix_sort radix_sort32/sort.comp )
add_shader( radix_sort radix_sort32/small_sort.comp )

