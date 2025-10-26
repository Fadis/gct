add_executable( flat_hair_blade
  gct.cpp
)
target_compile_definitions( flat_hair_blade PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( flat_hair_blade PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  flat_hair_blade
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

add_shader( flat_hair_blade mix_ao/mix_ao.comp )

add_shader( flat_hair_blade hair_blade/geometry.task )
add_shader( flat_hair_blade hair_blade/geometry.mesh )
add_shader( flat_hair_blade hair_blade/geometry.frag )

