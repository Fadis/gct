add_executable( dump_kplus_buffer_drop_small_triangles_partial
  gct.cpp
)
target_compile_definitions( dump_kplus_buffer_drop_small_triangles_partial PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( dump_kplus_buffer_drop_small_triangles_partial PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  dump_kplus_buffer_drop_small_triangles_partial
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

