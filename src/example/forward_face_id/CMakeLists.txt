add_executable( forward_face_id
  gct.cpp
)
target_compile_definitions( forward_face_id PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( forward_face_id PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  forward_face_id
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_shader( forward_face_id gamma/gamma.comp )

