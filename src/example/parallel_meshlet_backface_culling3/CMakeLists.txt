add_executable( parallel_meshlet_backface_culling3
  gct.cpp
)
target_compile_definitions( parallel_meshlet_backface_culling3 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( parallel_meshlet_backface_culling3 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  parallel_meshlet_backface_culling3
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

add_shader( parallel_meshlet_backface_culling3 shadow_mat/shadow_mat.comp )



add_shader( parallel_meshlet_backface_culling3 shadow/world.frag )
add_shader( parallel_meshlet_backface_culling3 shadow/world.geom )
add_shader( parallel_meshlet_backface_culling3 shadow/world.vert )

add_shader( parallel_meshlet_backface_culling3 skyview/generate_transmittance.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate_skyview.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate_multiscat.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/render.comp )

add_shader( parallel_meshlet_backface_culling3 geometry/geometry.task )
add_shader( parallel_meshlet_backface_culling3 geometry/geometry.mesh )
add_shader( parallel_meshlet_backface_culling3 geometry/geometry.frag )
add_shader( parallel_meshlet_backface_culling3 geometry/dummy.comp )

add_shader( parallel_meshlet_backface_culling3 aabb/world.frag )
add_shader( parallel_meshlet_backface_culling3 aabb/world.geom )
add_shader( parallel_meshlet_backface_culling3 aabb/world.vert )

add_shader( parallel_meshlet_backface_culling3 depth/world.frag )
add_shader( parallel_meshlet_backface_culling3 depth/world.vert )

add_shader( parallel_meshlet_backface_culling3 gauss/horizontal.comp )
add_shader( parallel_meshlet_backface_culling3 gauss/vertical.comp )

add_shader( parallel_meshlet_backface_culling3 lighting/lighting.comp )
add_shader( parallel_meshlet_backface_culling3 gamma/gamma.comp )

add_shader( parallel_meshlet_backface_culling3 nearest_position/nearest_position.comp )

add_shader( parallel_meshlet_backface_culling3 ao/hbao_0.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_45.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_90.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_135.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao.comp )

add_shader( parallel_meshlet_backface_culling3 mix_ao/mix_ao.comp )

add_shader( parallel_meshlet_backface_culling3 tone/tone.comp )


add_shader( parallel_meshlet_backface_culling3 distance/distance.comp )

add_shader( parallel_meshlet_backface_culling3 dof/h0.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h1.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h2.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h3.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h4.comp )
add_shader( parallel_meshlet_backface_culling3 dof/v.comp )

add_shader( parallel_meshlet_backface_culling3 merge/merge.comp )

add_shader( parallel_meshlet_backface_culling3 coc_gauss/horizontal.comp )
add_shader( parallel_meshlet_backface_culling3 coc_gauss/vertical.comp )

add_shader( parallel_meshlet_backface_culling3 sb/world.vert )
add_shader( parallel_meshlet_backface_culling3 sb/world.geom )
add_shader( parallel_meshlet_backface_culling3 sb/world.frag )
add_shader( parallel_meshlet_backface_culling3 flare/world.vert )
add_shader( parallel_meshlet_backface_culling3 flare/world.geom )
add_shader( parallel_meshlet_backface_culling3 flare/world.frag )

add_shader( parallel_meshlet_backface_culling3 meshlet_normal/meshlet_normal.comp )

