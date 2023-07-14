add_executable( dnn_eval
  dnn_eval.cpp
)
target_compile_definitions( dnn_eval PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( dnn_eval PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  dnn_eval
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

add_shader( dnn_eval shaders/linear_f32_f32_f32.comp )
add_shader( dnn_eval shaders/convolution_f32_f32_f32_f32.comp )
add_shader( dnn_eval shaders/relu_f32_f32.comp )
add_shader( dnn_eval shaders/softmax_f32_f32.comp )
add_shader( dnn_eval shaders/max_pool_f32_f32.comp )
