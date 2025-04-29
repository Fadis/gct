add_executable( restir
  gct.cpp
)
target_compile_definitions( restir PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( restir PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  restir
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

add_shader( restir shadow_mat/shadow_mat.comp )



add_shader( restir shadow/world.frag )
add_shader( restir shadow/world.geom )
add_shader( restir shadow/world.vert )

add_shader( restir skyview/generate_transmittance.comp )
add_shader( restir skyview/generate_skyview.comp )
add_shader( restir skyview/generate_multiscat.comp )
add_shader( restir skyview/froxel.comp )
add_shader( restir skyview/render_froxel.comp )

add_shader( restir geometry/tangent.frag )
add_shader( restir geometry/tangent.vert )
add_shader( restir geometry/world.frag )
add_shader( restir geometry/world.vert )

add_shader( restir voxel/world.frag )
add_shader( restir voxel/world.geom )
add_shader( restir voxel/world.vert )

add_shader( restir voronoi/voronoi.comp )

add_shader( restir distance_field/distance_field.comp )

add_shader( restir aabb/world.frag )
add_shader( restir aabb/world.geom )
add_shader( restir aabb/world.vert )

add_shader( restir depth/world.frag )
add_shader( restir depth/world.vert )

add_shader( restir matrix_pool/write.comp )
add_shader( restir matrix_pool/read.comp )
add_shader( restir matrix_pool/update.comp )
add_shader( restir matrix_pool/copy.comp )

add_shader( restir aabb_pool/write.comp )
add_shader( restir aabb_pool/read.comp )
add_shader( restir aabb_pool/update.comp )

add_shader( restir primitive_resource_index_pool/write.comp )
add_shader( restir primitive_resource_index_pool/read.comp )

add_shader( restir instance_resource_index_pool/write.comp )
add_shader( restir instance_resource_index_pool/read.comp )

add_shader( restir visibility_pool/write.comp )
add_shader( restir visibility_pool/read.comp )

add_shader( restir resource_pair/write.comp )
add_shader( restir resource_pair/read.comp )

add_shader( restir light_pool/write.comp )
add_shader( restir light_pool/read.comp )
add_shader( restir light_pool/update.comp )

add_shader( restir gauss/h5_16.comp )
add_shader( restir gauss/v5_16.comp )
add_shader( restir gauss/h12_32.comp )
add_shader( restir gauss/v12_32.comp )

add_shader( restir lighting/lighting.comp )
add_shader( restir lighting/hadaptive.comp )
add_shader( restir lighting/vadaptive.comp )
add_shader( restir lighting/direct_light.comp )

add_shader( restir wrs/wrs.comp )

add_shader( restir gamma/gamma.comp )

#add_shader( restir ao/hbao_0.comp )
#add_shader( restir ao/hbao_45.comp )
#add_shader( restir ao/hbao_90.comp )
#add_shader( restir ao/hbao_135.comp )
#add_shader( restir ao/hbao.comp )

add_shader( restir ao/rtao.comp )
add_shader( restir ao/hadaptive.comp )
add_shader( restir ao/vadaptive.comp )

add_shader( restir mix_ao/mix_ao.comp )

add_shader( restir tone/tone.comp )
