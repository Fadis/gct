add_executable( meshlet_backface_culling
  gct.cpp
)
target_compile_definitions( meshlet_backface_culling PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( meshlet_backface_culling PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  meshlet_backface_culling
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

add_shader( meshlet_backface_culling shadow_mat/shadow_mat.comp )



add_shader( meshlet_backface_culling shadow/world.frag )
add_shader( meshlet_backface_culling shadow/world.geom )
add_shader( meshlet_backface_culling shadow/world.vert )

add_shader( meshlet_backface_culling skyview/generate_transmittance.comp )
add_shader( meshlet_backface_culling skyview/generate_skyview.comp )
add_shader( meshlet_backface_culling skyview/generate_multiscat.comp )
add_shader( meshlet_backface_culling skyview/generate.comp )
add_shader( meshlet_backface_culling skyview/render.comp )

add_shader( meshlet_backface_culling geometry/geometry.task )
add_shader( meshlet_backface_culling geometry/geometry.mesh )
add_shader( meshlet_backface_culling geometry/geometry.frag )
add_shader( meshlet_backface_culling geometry/dummy.comp )

add_shader( meshlet_backface_culling aabb/world.frag )
add_shader( meshlet_backface_culling aabb/world.geom )
add_shader( meshlet_backface_culling aabb/world.vert )

add_shader( meshlet_backface_culling depth/world.frag )
add_shader( meshlet_backface_culling depth/world.vert )

add_shader( meshlet_backface_culling matrix_pool/write.comp )
add_shader( meshlet_backface_culling matrix_pool/read.comp )
add_shader( meshlet_backface_culling matrix_pool/update.comp )
add_shader( meshlet_backface_culling matrix_pool/copy.comp )

add_shader( meshlet_backface_culling aabb_pool/write.comp )
add_shader( meshlet_backface_culling aabb_pool/read.comp )
add_shader( meshlet_backface_culling aabb_pool/update.comp )

add_shader( meshlet_backface_culling image_pool/rgba8.comp )
add_shader( meshlet_backface_culling image_pool/rgba16.comp )
add_shader( meshlet_backface_culling image_pool/rgba16f.comp )
add_shader( meshlet_backface_culling image_pool/rgba32f.comp )

add_shader( meshlet_backface_culling primitive_resource_index_pool/write.comp )
add_shader( meshlet_backface_culling primitive_resource_index_pool/read.comp )

add_shader( meshlet_backface_culling instance_resource_index_pool/write.comp )
add_shader( meshlet_backface_culling instance_resource_index_pool/read.comp )

add_shader( meshlet_backface_culling visibility_pool/write.comp )
add_shader( meshlet_backface_culling visibility_pool/read.comp )

add_shader( meshlet_backface_culling resource_pair/write.comp )
add_shader( meshlet_backface_culling resource_pair/read.comp )

add_shader( meshlet_backface_culling light_pool/write.comp )
add_shader( meshlet_backface_culling light_pool/read.comp )
add_shader( meshlet_backface_culling light_pool/update.comp )

add_shader( meshlet_backface_culling accessor/write.comp )
add_shader( meshlet_backface_culling accessor/read.comp )

add_shader( meshlet_backface_culling mesh/write.comp )
add_shader( meshlet_backface_culling mesh/read.comp )

add_shader( meshlet_backface_culling meshlet/write.comp )
add_shader( meshlet_backface_culling meshlet/read.comp )

add_shader( meshlet_backface_culling meshlet_index/write.comp )
add_shader( meshlet_backface_culling meshlet_index/read.comp )

add_shader( meshlet_backface_culling gauss/horizontal.comp )
add_shader( meshlet_backface_culling gauss/vertical.comp )

add_shader( meshlet_backface_culling lighting/lighting.comp )
add_shader( meshlet_backface_culling gamma/gamma.comp )

add_shader( meshlet_backface_culling nearest_position/nearest_position.comp )

add_shader( meshlet_backface_culling ao/hbao_0.comp )
add_shader( meshlet_backface_culling ao/hbao_45.comp )
add_shader( meshlet_backface_culling ao/hbao_90.comp )
add_shader( meshlet_backface_culling ao/hbao_135.comp )
add_shader( meshlet_backface_culling ao/hbao.comp )

add_shader( meshlet_backface_culling mix_ao/mix_ao.comp )

add_shader( meshlet_backface_culling tone/tone.comp )


add_shader( meshlet_backface_culling distance/distance.comp )

add_shader( meshlet_backface_culling dof/h0.comp )
add_shader( meshlet_backface_culling dof/h1.comp )
add_shader( meshlet_backface_culling dof/h2.comp )
add_shader( meshlet_backface_culling dof/h3.comp )
add_shader( meshlet_backface_culling dof/h4.comp )
add_shader( meshlet_backface_culling dof/v.comp )

add_shader( meshlet_backface_culling merge/merge.comp )

add_shader( meshlet_backface_culling coc_gauss/horizontal.comp )
add_shader( meshlet_backface_culling coc_gauss/vertical.comp )

add_shader( meshlet_backface_culling sb/world.vert )
add_shader( meshlet_backface_culling sb/world.geom )
add_shader( meshlet_backface_culling sb/world.frag )
add_shader( meshlet_backface_culling flare/world.vert )
add_shader( meshlet_backface_culling flare/world.geom )
add_shader( meshlet_backface_culling flare/world.frag )

add_shader( meshlet_backface_culling meshlet_normal/meshlet_normal.comp )

