add_executable( flat_terrain
  gct.cpp
)
target_compile_definitions( flat_terrain PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( flat_terrain PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  flat_terrain
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

add_shader( flat_terrain shadow_mat/shadow_mat.comp )



add_shader( flat_terrain shadow/world.frag )
add_shader( flat_terrain shadow/world.geom )
add_shader( flat_terrain shadow/world.vert )

add_shader( flat_terrain skyview/generate_transmittance.comp )
add_shader( flat_terrain skyview/generate_skyview.comp )
add_shader( flat_terrain skyview/generate_multiscat.comp )
add_shader( flat_terrain skyview/generate.comp )
add_shader( flat_terrain skyview/render.comp )

add_shader( flat_terrain geometry/geometry.task )
add_shader( flat_terrain geometry/geometry.mesh )
add_shader( flat_terrain geometry/geometry.frag )
add_shader( flat_terrain geometry/dummy.comp )

add_shader( flat_terrain terrain/terrain.task )
add_shader( flat_terrain terrain/terrain.mesh )
add_shader( flat_terrain terrain/terrain.frag )

add_shader( flat_terrain aabb/world.frag )
add_shader( flat_terrain aabb/world.geom )
add_shader( flat_terrain aabb/world.vert )

add_shader( flat_terrain depth/world.frag )
add_shader( flat_terrain depth/world.vert )

add_shader( flat_terrain gauss/horizontal.comp )
add_shader( flat_terrain gauss/vertical.comp )

add_shader( flat_terrain lighting/lighting.comp )
add_shader( flat_terrain gamma/gamma.comp )

add_shader( flat_terrain nearest_position/nearest_position.comp )

add_shader( flat_terrain ao/hbao_0.comp )
add_shader( flat_terrain ao/hbao_45.comp )
add_shader( flat_terrain ao/hbao_90.comp )
add_shader( flat_terrain ao/hbao_135.comp )
add_shader( flat_terrain ao/hbao.comp )

add_shader( flat_terrain mix_ao/mix_ao.comp )

add_shader( flat_terrain tone/tone.comp )


add_shader( flat_terrain distance/distance.comp )

add_shader( flat_terrain dof/h0.comp )
add_shader( flat_terrain dof/h1.comp )
add_shader( flat_terrain dof/h2.comp )
add_shader( flat_terrain dof/h3.comp )
add_shader( flat_terrain dof/h4.comp )
add_shader( flat_terrain dof/v.comp )

add_shader( flat_terrain merge/merge.comp )

add_shader( flat_terrain coc_gauss/horizontal.comp )
add_shader( flat_terrain coc_gauss/vertical.comp )

add_shader( flat_terrain sb/world.vert )
add_shader( flat_terrain sb/world.geom )
add_shader( flat_terrain sb/world.frag )
add_shader( flat_terrain flare/world.vert )
add_shader( flat_terrain flare/world.geom )
add_shader( flat_terrain flare/world.frag )

add_shader( flat_terrain meshlet_normal/meshlet_normal.comp )

