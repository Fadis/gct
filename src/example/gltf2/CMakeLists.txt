add_executable( gltf2
  gct.cpp
)
target_compile_definitions( gltf2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( gltf2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  gltf2
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

add_shader( gltf2 shadow_mat/shadow_mat.comp )



add_shader( gltf2 shadow/world.frag )
add_shader( gltf2 shadow/world.geom )
add_shader( gltf2 shadow/world.vert )

add_shader( gltf2 geometry/tangent.frag )
add_shader( gltf2 geometry/tangent.vert )
add_shader( gltf2 geometry/world.frag )
add_shader( gltf2 geometry/world.vert )

add_shader( gltf2 geometry_test/world.frag )
add_shader( gltf2 geometry_test/world.vert )
add_shader( gltf2 geometry_test/world.geom )


add_shader( gltf2 matrix_pool/write.comp )
add_shader( gltf2 matrix_pool/read.comp )
add_shader( gltf2 matrix_pool/update.comp )

add_shader( gltf2 aabb_pool/write.comp )
add_shader( gltf2 aabb_pool/read.comp )
add_shader( gltf2 aabb_pool/update.comp )

add_shader( gltf2 primitive_resource_index_pool/write.comp )
add_shader( gltf2 primitive_resource_index_pool/read.comp )

add_shader( gltf2 instance_resource_index_pool/write.comp )
add_shader( gltf2 instance_resource_index_pool/read.comp )

add_shader( gltf2 visibility_pool/write.comp )
add_shader( gltf2 visibility_pool/read.comp )

add_shader( gltf2 gauss/h5_16.comp )
add_shader( gltf2 gauss/v5_16.comp )
add_shader( gltf2 gauss/h12_32.comp )
add_shader( gltf2 gauss/v12_32.comp )

add_shader( gltf2 lighting/lighting.comp )
add_shader( gltf2 gamma/gamma.comp )

add_shader( gltf2 ao/hbao_0.comp )
add_shader( gltf2 ao/hbao_45.comp )
add_shader( gltf2 ao/hbao_90.comp )
add_shader( gltf2 ao/hbao_135.comp )
add_shader( gltf2 ao/hbao.comp )

add_shader( gltf2 mix_ao/mix_ao.comp )

add_shader( gltf2 tone/tone.comp )
