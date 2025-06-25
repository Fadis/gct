add_executable( lod
  gct.cpp
)
target_compile_definitions( lod PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( lod PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  lod
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

add_shader( lod shadow_mat/shadow_mat.comp )



add_shader( lod shadow/world.frag )
add_shader( lod shadow/world.geom )
add_shader( lod shadow/world.vert )

add_shader( lod skyview/generate_transmittance.comp )
add_shader( lod skyview/generate_skyview.comp )
add_shader( lod skyview/generate_multiscat.comp )
add_shader( lod skyview/generate.comp )
add_shader( lod skyview/render.comp )

add_shader( lod geometry/geometry.task )
add_shader( lod geometry/geometry.mesh )
add_shader( lod geometry/geometry.frag )
add_shader( lod geometry/dummy.comp )

add_shader( lod aabb/world.frag )
add_shader( lod aabb/world.geom )
add_shader( lod aabb/world.vert )

add_shader( lod depth/world.frag )
add_shader( lod depth/world.vert )

add_shader( lod gauss/horizontal.comp )
add_shader( lod gauss/vertical.comp )

add_shader( lod lighting/lighting.comp )
add_shader( lod gamma/gamma.comp )

add_shader( lod nearest_position/nearest_position.comp )

add_shader( lod ao/hbao_0.comp )
add_shader( lod ao/hbao_45.comp )
add_shader( lod ao/hbao_90.comp )
add_shader( lod ao/hbao_135.comp )
add_shader( lod ao/hbao.comp )

add_shader( lod mix_ao/mix_ao.comp )

add_shader( lod tone/tone.comp )


add_shader( lod distance/distance.comp )

add_shader( lod dof/h0.comp )
add_shader( lod dof/h1.comp )
add_shader( lod dof/h2.comp )
add_shader( lod dof/h3.comp )
add_shader( lod dof/h4.comp )
add_shader( lod dof/v.comp )

add_shader( lod merge/merge.comp )

add_shader( lod coc_gauss/horizontal.comp )
add_shader( lod coc_gauss/vertical.comp )

add_shader( lod sb/world.vert )
add_shader( lod sb/world.geom )
add_shader( lod sb/world.frag )
add_shader( lod flare/world.vert )
add_shader( lod flare/world.geom )
add_shader( lod flare/world.frag )

add_shader( lod meshlet_normal/meshlet_normal.comp )

