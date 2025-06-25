add_executable( bilateral
  gct.cpp
)
target_compile_definitions( bilateral PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( bilateral PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  bilateral
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

add_shader( bilateral shadow_mat/shadow_mat.comp )



add_shader( bilateral shadow/world.frag )
add_shader( bilateral shadow/world.geom )
add_shader( bilateral shadow/world.vert )

add_shader( bilateral skyview/generate_transmittance.comp )
add_shader( bilateral skyview/generate_skyview.comp )
add_shader( bilateral skyview/generate_multiscat.comp )
add_shader( bilateral skyview/generate.comp )
add_shader( bilateral skyview/render.comp )

add_shader( bilateral geometry/tangent.frag )
add_shader( bilateral geometry/tangent.vert )
add_shader( bilateral geometry/world.frag )
add_shader( bilateral geometry/world.vert )
add_shader( bilateral geometry/dummy.comp )

add_shader( bilateral aabb/world.frag )
add_shader( bilateral aabb/world.geom )
add_shader( bilateral aabb/world.vert )

add_shader( bilateral depth/world.frag )
add_shader( bilateral depth/world.vert )

add_shader( bilateral gauss/horizontal.comp )
add_shader( bilateral gauss/vertical.comp )

add_shader( bilateral lighting/lighting.comp )
add_shader( bilateral gamma/gamma.comp )

add_shader( bilateral nearest_position/nearest_position.comp )

add_shader( bilateral ao/hbao_0.comp )
add_shader( bilateral ao/hbao_45.comp )
add_shader( bilateral ao/hbao_90.comp )
add_shader( bilateral ao/hbao_135.comp )
add_shader( bilateral ao/hbao.comp )

add_shader( bilateral mix_ao/mix_ao.comp )

add_shader( bilateral tone/tone.comp )


add_shader( bilateral distance/distance.comp )

add_shader( bilateral dof/h0.comp )
add_shader( bilateral dof/h1.comp )
add_shader( bilateral dof/h2.comp )
add_shader( bilateral dof/h3.comp )
add_shader( bilateral dof/h4.comp )
add_shader( bilateral dof/v.comp )

add_shader( bilateral merge/merge.comp )

add_shader( bilateral coc_gauss/horizontal.comp )
add_shader( bilateral coc_gauss/vertical.comp )

add_shader( bilateral sb/world.vert )
add_shader( bilateral sb/world.geom )
add_shader( bilateral sb/world.frag )
add_shader( bilateral flare/world.vert )
add_shader( bilateral flare/world.geom )
add_shader( bilateral flare/world.frag )

add_shader( bilateral bilateral_weight/bilateral_weight.comp )
