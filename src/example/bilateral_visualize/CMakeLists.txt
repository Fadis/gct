add_executable( bilateral_visualize
  gct.cpp
)
target_compile_definitions( bilateral_visualize PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( bilateral_visualize PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  bilateral_visualize
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

add_shader( bilateral_visualize shadow_mat/shadow_mat.comp )



add_shader( bilateral_visualize shadow/world.frag )
add_shader( bilateral_visualize shadow/world.geom )
add_shader( bilateral_visualize shadow/world.vert )

add_shader( bilateral_visualize skyview/generate_transmittance.comp )
add_shader( bilateral_visualize skyview/generate_skyview.comp )
add_shader( bilateral_visualize skyview/generate_multiscat.comp )
add_shader( bilateral_visualize skyview/generate.comp )
add_shader( bilateral_visualize skyview/render.comp )

add_shader( bilateral_visualize geometry/tangent.frag )
add_shader( bilateral_visualize geometry/tangent.vert )
add_shader( bilateral_visualize geometry/world.frag )
add_shader( bilateral_visualize geometry/world.vert )
add_shader( bilateral_visualize geometry/dummy.comp )

add_shader( bilateral_visualize aabb/world.frag )
add_shader( bilateral_visualize aabb/world.geom )
add_shader( bilateral_visualize aabb/world.vert )

add_shader( bilateral_visualize depth/world.frag )
add_shader( bilateral_visualize depth/world.vert )

add_shader( bilateral_visualize gauss/horizontal.comp )
add_shader( bilateral_visualize gauss/vertical.comp )

add_shader( bilateral_visualize lighting/lighting.comp )
add_shader( bilateral_visualize gamma/gamma.comp )

add_shader( bilateral_visualize nearest_position/nearest_position.comp )

add_shader( bilateral_visualize ao/hbao_0.comp )
add_shader( bilateral_visualize ao/hbao_45.comp )
add_shader( bilateral_visualize ao/hbao_90.comp )
add_shader( bilateral_visualize ao/hbao_135.comp )
add_shader( bilateral_visualize ao/hbao.comp )

add_shader( bilateral_visualize mix_ao/mix_ao.comp )

add_shader( bilateral_visualize tone/tone.comp )


add_shader( bilateral_visualize distance/distance.comp )

add_shader( bilateral_visualize dof/h0.comp )
add_shader( bilateral_visualize dof/h1.comp )
add_shader( bilateral_visualize dof/h2.comp )
add_shader( bilateral_visualize dof/h3.comp )
add_shader( bilateral_visualize dof/h4.comp )
add_shader( bilateral_visualize dof/v.comp )

add_shader( bilateral_visualize merge/merge.comp )

add_shader( bilateral_visualize coc_gauss/horizontal.comp )
add_shader( bilateral_visualize coc_gauss/vertical.comp )

add_shader( bilateral_visualize sb/world.vert )
add_shader( bilateral_visualize sb/world.geom )
add_shader( bilateral_visualize sb/world.frag )
add_shader( bilateral_visualize flare/world.vert )
add_shader( bilateral_visualize flare/world.geom )
add_shader( bilateral_visualize flare/world.frag )

add_shader( bilateral_visualize bilateral_weight/bilateral_weight.comp )
