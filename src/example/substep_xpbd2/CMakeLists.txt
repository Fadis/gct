add_executable( substep_xpbd2
  gct.cpp
)
target_compile_definitions( substep_xpbd2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( substep_xpbd2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  substep_xpbd2
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

add_shader( substep_xpbd2 mix_ao/mix_ao.comp )

add_shader( substep_xpbd2 attach_particle/attach_particle.comp )


