add_executable( shell_and_fin
  gct.cpp
)
target_compile_definitions( shell_and_fin PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( shell_and_fin PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  shell_and_fin
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

add_shader( shell_and_fin mix_ao/mix_ao.comp )

add_shader( shell_and_fin fur/geometry.task INSTALL )
add_shader( shell_and_fin fur/geometry.mesh INSTALL )
add_shader( shell_and_fin fur/geometry.frag INSTALL )
