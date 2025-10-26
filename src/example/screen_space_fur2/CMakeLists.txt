add_executable( screen_space_fur2
  gct.cpp
)
target_compile_definitions( screen_space_fur2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( screen_space_fur2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  screen_space_fur2
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

add_shader( screen_space_fur2 mix_ao/mix_ao.comp )

add_shader( screen_space_fur2 fur/geometry.task INSTALL )
add_shader( screen_space_fur2 fur/geometry.mesh INSTALL )
add_shader( screen_space_fur2 fur/geometry.frag INSTALL )
