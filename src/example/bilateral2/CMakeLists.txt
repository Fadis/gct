add_executable( bilateral2
  gct.cpp
)
target_compile_definitions( bilateral2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( bilateral2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  bilateral2
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

add_shader( bilateral2 shadow_mat/shadow_mat.comp )



add_shader( bilateral2 shadow/world.frag )
add_shader( bilateral2 shadow/world.geom )
add_shader( bilateral2 shadow/world.vert )

add_shader( bilateral2 skyview/generate_transmittance.comp )
add_shader( bilateral2 skyview/generate_skyview.comp )
add_shader( bilateral2 skyview/generate_multiscat.comp )
add_shader( bilateral2 skyview/generate.comp )
add_shader( bilateral2 skyview/render.comp )

add_shader( bilateral2 geometry/tangent.frag )
add_shader( bilateral2 geometry/tangent.vert )
add_shader( bilateral2 geometry/world.frag )
add_shader( bilateral2 geometry/world.vert )
add_shader( bilateral2 geometry/dummy.comp )

add_shader( bilateral2 aabb/world.frag )
add_shader( bilateral2 aabb/world.geom )
add_shader( bilateral2 aabb/world.vert )

add_shader( bilateral2 depth/world.frag )
add_shader( bilateral2 depth/world.vert )

add_shader( bilateral2 gauss/horizontal.comp )
add_shader( bilateral2 gauss/vertical.comp )

add_shader( bilateral2 lighting/lighting.comp )
add_shader( bilateral2 gamma/gamma.comp )

add_shader( bilateral2 nearest_position/nearest_position.comp )

add_shader( bilateral2 ao/hbao_0.comp )
add_shader( bilateral2 ao/hbao_45.comp )
add_shader( bilateral2 ao/hbao_90.comp )
add_shader( bilateral2 ao/hbao_135.comp )
add_shader( bilateral2 ao/hbao.comp )

add_shader( bilateral2 mix_ao/mix_ao.comp )

add_shader( bilateral2 tone/tone.comp )


add_shader( bilateral2 distance/distance.comp )

add_shader( bilateral2 dof/h0.comp )
add_shader( bilateral2 dof/h1.comp )
add_shader( bilateral2 dof/h2.comp )
add_shader( bilateral2 dof/h3.comp )
add_shader( bilateral2 dof/h4.comp )
add_shader( bilateral2 dof/v.comp )

add_shader( bilateral2 merge/merge.comp )

add_shader( bilateral2 coc_gauss/horizontal.comp )
add_shader( bilateral2 coc_gauss/vertical.comp )

add_shader( bilateral2 sb/world.vert )
add_shader( bilateral2 sb/world.geom )
add_shader( bilateral2 sb/world.frag )
add_shader( bilateral2 flare/world.vert )
add_shader( bilateral2 flare/world.geom )
add_shader( bilateral2 flare/world.frag )

add_shader( bilateral2 bilateral_weight/bilateral_weight.comp )
