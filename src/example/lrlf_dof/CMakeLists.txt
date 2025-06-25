add_executable( lrlf_dof
  gct.cpp
)
target_compile_definitions( lrlf_dof PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( lrlf_dof PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  lrlf_dof
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

add_shader( lrlf_dof shadow_mat/shadow_mat.comp )



add_shader( lrlf_dof shadow/world.frag )
add_shader( lrlf_dof shadow/world.geom )
add_shader( lrlf_dof shadow/world.vert )

add_shader( lrlf_dof skyview/generate_transmittance.comp )
add_shader( lrlf_dof skyview/generate_skyview.comp )
add_shader( lrlf_dof skyview/generate_multiscat.comp )
add_shader( lrlf_dof skyview/generate.comp )
add_shader( lrlf_dof skyview/render.comp )

add_shader( lrlf_dof geometry/tangent.frag )
add_shader( lrlf_dof geometry/tangent.vert )
add_shader( lrlf_dof geometry/world.frag )
add_shader( lrlf_dof geometry/world.vert )
add_shader( lrlf_dof geometry/dummy.comp )

add_shader( lrlf_dof aabb/world.frag )
add_shader( lrlf_dof aabb/world.geom )
add_shader( lrlf_dof aabb/world.vert )

add_shader( lrlf_dof depth/world.frag )
add_shader( lrlf_dof depth/world.vert )

add_shader( lrlf_dof gauss/horizontal.comp )
add_shader( lrlf_dof gauss/vertical.comp )

add_shader( lrlf_dof lighting/lighting.comp )
add_shader( lrlf_dof gamma/gamma.comp )

add_shader( lrlf_dof nearest_position/nearest_position.comp )

add_shader( lrlf_dof ao/hbao_0.comp )
add_shader( lrlf_dof ao/hbao_45.comp )
add_shader( lrlf_dof ao/hbao_90.comp )
add_shader( lrlf_dof ao/hbao_135.comp )
add_shader( lrlf_dof ao/hbao.comp )

add_shader( lrlf_dof mix_ao/mix_ao.comp )

add_shader( lrlf_dof tone/tone.comp )


add_shader( lrlf_dof distance/distance.comp )

add_shader( lrlf_dof dof/h0.comp )
add_shader( lrlf_dof dof/h1.comp )
add_shader( lrlf_dof dof/h2.comp )
add_shader( lrlf_dof dof/h3.comp )
add_shader( lrlf_dof dof/h4.comp )
add_shader( lrlf_dof dof/v.comp )

add_shader( lrlf_dof merge/merge.comp )

add_shader( lrlf_dof coc_gauss/horizontal.comp )
add_shader( lrlf_dof coc_gauss/vertical.comp )

add_shader( lrlf_dof sb/world.vert )
add_shader( lrlf_dof sb/world.geom )
add_shader( lrlf_dof sb/world.frag )
add_shader( lrlf_dof flare/world.vert )
add_shader( lrlf_dof flare/world.geom )
add_shader( lrlf_dof flare/world.frag )
