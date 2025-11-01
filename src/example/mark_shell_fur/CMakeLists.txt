add_executable( mark_shell_fur
  gct.cpp
)
target_compile_definitions( mark_shell_fur PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( mark_shell_fur PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  mark_shell_fur
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

add_shader( mark_shell_fur shell/geometry.task )
add_shader( mark_shell_fur shell/geometry.mesh )
add_shader( mark_shell_fur shell/geometry.frag )

