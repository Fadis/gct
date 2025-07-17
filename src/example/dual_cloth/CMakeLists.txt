add_executable( dual_cloth
  gct.cpp
)
target_compile_definitions( dual_cloth PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( dual_cloth PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  dual_cloth
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

add_shader( dual_cloth shadow_mat/shadow_mat.comp )

add_shader( dual_cloth shadow/world.frag )
add_shader( dual_cloth shadow/world.geom )
add_shader( dual_cloth shadow/world.vert )

add_shader( dual_cloth aabb/world.frag )
add_shader( dual_cloth aabb/world.geom )
add_shader( dual_cloth aabb/world.vert )

add_shader( dual_cloth depth/world.frag )
add_shader( dual_cloth depth/world.vert )

add_shader( dual_cloth gauss/horizontal.comp )
add_shader( dual_cloth gauss/vertical.comp )

add_shader( dual_cloth lighting/lighting.comp )
add_shader( dual_cloth gamma/gamma.comp )

add_shader( dual_cloth nearest_position/nearest_position.comp )

add_shader( dual_cloth mix_ao/mix_ao.comp )

add_shader( dual_cloth tone/tone.comp )


add_shader( dual_cloth distance/distance.comp )

add_shader( dual_cloth merge/merge.comp )

add_shader( dual_cloth coc_gauss/horizontal.comp )
add_shader( dual_cloth coc_gauss/vertical.comp )

add_shader( dual_cloth sb/world.vert )
add_shader( dual_cloth sb/world.geom )
add_shader( dual_cloth sb/world.frag )
add_shader( dual_cloth flare/world.vert )
add_shader( dual_cloth flare/world.geom )
add_shader( dual_cloth flare/world.frag )

add_shader( dual_cloth meshlet_normal/meshlet_normal.comp )

add_shader( dual_cloth attach_particle/attach_particle.comp )


