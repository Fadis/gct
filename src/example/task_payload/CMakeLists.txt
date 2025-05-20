add_executable( task_payload
  gct.cpp
)
target_compile_definitions( task_payload PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( task_payload PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  task_payload
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

add_shader( task_payload geometry/geometry.task )
add_shader( task_payload geometry/geometry.mesh )
add_shader( task_payload geometry/geometry.frag )
add_shader( task_payload geometry/dummy.comp )
add_shader( task_payload gamma/gamma.comp )

