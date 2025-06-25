add_executable( half_resolution
  gct.cpp
)
target_compile_definitions( half_resolution PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( half_resolution PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  half_resolution
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

add_shader( half_resolution shadow_mat/shadow_mat.comp )



add_shader( half_resolution shadow/world.frag )
add_shader( half_resolution shadow/world.geom )
add_shader( half_resolution shadow/world.vert )

add_shader( half_resolution skyview/generate_transmittance.comp )
add_shader( half_resolution skyview/generate_skyview.comp )
add_shader( half_resolution skyview/generate_multiscat.comp )
add_shader( half_resolution skyview/generate.comp )
add_shader( half_resolution skyview/render.comp )

add_shader( half_resolution geometry/tangent.frag )
add_shader( half_resolution geometry/tangent.vert )
add_shader( half_resolution geometry/world.frag )
add_shader( half_resolution geometry/world.vert )
add_shader( half_resolution geometry/dummy.comp )

add_shader( half_resolution aabb/world.frag )
add_shader( half_resolution aabb/world.geom )
add_shader( half_resolution aabb/world.vert )

add_shader( half_resolution depth/world.frag )
add_shader( half_resolution depth/world.vert )

add_shader( half_resolution gauss/horizontal.comp )
add_shader( half_resolution gauss/vertical.comp )

add_shader( half_resolution lighting/lighting.comp )
add_shader( half_resolution gamma/gamma.comp )

add_shader( half_resolution nearest_position/nearest_position.comp )

add_shader( half_resolution ao/hbao_0.comp )
add_shader( half_resolution ao/hbao_45.comp )
add_shader( half_resolution ao/hbao_90.comp )
add_shader( half_resolution ao/hbao_135.comp )
add_shader( half_resolution ao/hbao.comp )

add_shader( half_resolution mix_ao/mix_ao.comp )

add_shader( half_resolution tone/tone.comp )


add_shader( half_resolution distance/distance.comp )

add_shader( half_resolution dof/h0.comp )
add_shader( half_resolution dof/h1.comp )
add_shader( half_resolution dof/h2.comp )
add_shader( half_resolution dof/h3.comp )
add_shader( half_resolution dof/h4.comp )
add_shader( half_resolution dof/v.comp )

add_shader( half_resolution merge/merge.comp )

add_shader( half_resolution coc_gauss/horizontal.comp )
add_shader( half_resolution coc_gauss/vertical.comp )

add_shader( half_resolution sb/world.vert )
add_shader( half_resolution sb/world.geom )
add_shader( half_resolution sb/world.frag )
add_shader( half_resolution flare/world.vert )
add_shader( half_resolution flare/world.geom )
add_shader( half_resolution flare/world.frag )
