add_executable( drop_small_triangles
  gct.cpp
)
target_compile_definitions( drop_small_triangles PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( drop_small_triangles PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  drop_small_triangles
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( drop_small_triangles mix_ao/mix_ao.comp )

