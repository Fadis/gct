add_executable( incomplete_softrawst
  gct.cpp
)
target_compile_definitions( incomplete_softrawst PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( incomplete_softrawst PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  incomplete_softrawst
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( incomplete_softrawst mix_ao/mix_ao.comp )

