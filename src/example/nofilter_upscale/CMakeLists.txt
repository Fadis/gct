add_executable( nofilter_upscale
  gct.cpp
)
target_compile_definitions( nofilter_upscale PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( nofilter_upscale PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  nofilter_upscale
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

add_shader( nofilter_upscale shadow_mat/shadow_mat.comp )



add_shader( nofilter_upscale shadow/world.frag )
add_shader( nofilter_upscale shadow/world.geom )
add_shader( nofilter_upscale shadow/world.vert )

add_shader( nofilter_upscale skyview/generate_transmittance.comp )
add_shader( nofilter_upscale skyview/generate_skyview.comp )
add_shader( nofilter_upscale skyview/generate_multiscat.comp )
add_shader( nofilter_upscale skyview/generate.comp )
add_shader( nofilter_upscale skyview/render.comp )

add_shader( nofilter_upscale geometry/tangent.frag )
add_shader( nofilter_upscale geometry/tangent.vert )
add_shader( nofilter_upscale geometry/world.frag )
add_shader( nofilter_upscale geometry/world.vert )
add_shader( nofilter_upscale geometry/dummy.comp )

add_shader( nofilter_upscale aabb/world.frag )
add_shader( nofilter_upscale aabb/world.geom )
add_shader( nofilter_upscale aabb/world.vert )

add_shader( nofilter_upscale depth/world.frag )
add_shader( nofilter_upscale depth/world.vert )

add_shader( nofilter_upscale gauss/horizontal.comp )
add_shader( nofilter_upscale gauss/vertical.comp )

add_shader( nofilter_upscale lighting/lighting.comp )
add_shader( nofilter_upscale gamma/gamma.comp )

add_shader( nofilter_upscale nearest_position/nearest_position.comp )

add_shader( nofilter_upscale ao/hbao_0.comp )
add_shader( nofilter_upscale ao/hbao_45.comp )
add_shader( nofilter_upscale ao/hbao_90.comp )
add_shader( nofilter_upscale ao/hbao_135.comp )
add_shader( nofilter_upscale ao/hbao.comp )

add_shader( nofilter_upscale mix_ao/mix_ao.comp )

add_shader( nofilter_upscale tone/tone.comp )


add_shader( nofilter_upscale distance/distance.comp )

add_shader( nofilter_upscale dof/h0.comp )
add_shader( nofilter_upscale dof/h1.comp )
add_shader( nofilter_upscale dof/h2.comp )
add_shader( nofilter_upscale dof/h3.comp )
add_shader( nofilter_upscale dof/h4.comp )
add_shader( nofilter_upscale dof/v.comp )

add_shader( nofilter_upscale merge/merge.comp )

add_shader( nofilter_upscale coc_gauss/horizontal.comp )
add_shader( nofilter_upscale coc_gauss/vertical.comp )

add_shader( nofilter_upscale sb/world.vert )
add_shader( nofilter_upscale sb/world.geom )
add_shader( nofilter_upscale sb/world.frag )
add_shader( nofilter_upscale flare/world.vert )
add_shader( nofilter_upscale flare/world.geom )
add_shader( nofilter_upscale flare/world.frag )
