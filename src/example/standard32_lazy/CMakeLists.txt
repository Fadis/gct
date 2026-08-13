add_executable( standard32_lazy
  gct.cpp
)
target_compile_definitions( standard32_lazy PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( standard32_lazy PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  standard32_lazy
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( standard32_lazy mix_ao/mix_ao.comp )

