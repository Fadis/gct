add_executable( image_io
  gct.cpp
)
target_compile_definitions( image_io PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( image_io PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  image_io
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

add_shader( image_io matrix_pool/write.comp )
add_shader( image_io matrix_pool/read.comp )
add_shader( image_io matrix_pool/update.comp )
add_shader( image_io matrix_pool/copy.comp )

add_shader( image_io aabb_pool/write.comp )
add_shader( image_io aabb_pool/read.comp )
add_shader( image_io aabb_pool/update.comp )

add_shader( image_io image_pool/rgba8.comp )
add_shader( image_io image_pool/rgba16.comp )
add_shader( image_io image_pool/rgba16f.comp )
add_shader( image_io image_pool/rgba32f.comp )

add_shader( image_io primitive_resource_index_pool/write.comp )
add_shader( image_io primitive_resource_index_pool/read.comp )

add_shader( image_io instance_resource_index_pool/write.comp )
add_shader( image_io instance_resource_index_pool/read.comp )

add_shader( image_io visibility_pool/write.comp )
add_shader( image_io visibility_pool/read.comp )

add_shader( image_io light_pool/write.comp )
add_shader( image_io light_pool/read.comp )
add_shader( image_io light_pool/update.comp )

add_shader( image_io dummy/dummy.comp )

add_shader( image_io shrink/shrink.comp )
add_shader( image_io rotate/rotate.comp )

