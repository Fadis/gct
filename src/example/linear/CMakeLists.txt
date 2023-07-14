add_executable( linear
  linear.cpp
)
target_compile_definitions( linear PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( linear PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  linear
  gct
  gct_dnn
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( linear shaders/linear_f32_f32_f32.comp )
add_shader( linear shaders/convolution_f32_f32_f32_f32.comp )
add_shader( linear shaders/relu_f32_f32.comp )
add_shader( linear shaders/softmax_f32_f32.comp )
add_shader( linear shaders/max_pool_f32_f32.comp )
