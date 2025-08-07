add_executable( single_cloth
  gct.cpp
)
target_compile_definitions( single_cloth PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( single_cloth PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  single_cloth
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

add_shader( single_cloth shadow_mat/shadow_mat.comp )

add_shader( single_cloth shadow/world.frag )
add_shader( single_cloth shadow/world.geom )
add_shader( single_cloth shadow/world.vert )

add_shader( single_cloth aabb/world.frag )
add_shader( single_cloth aabb/world.geom )
add_shader( single_cloth aabb/world.vert )

add_shader( single_cloth depth/world.frag )
add_shader( single_cloth depth/world.vert )

add_shader( single_cloth gauss/horizontal.comp )
add_shader( single_cloth gauss/vertical.comp )

add_shader( single_cloth lighting/lighting.comp )
add_shader( single_cloth gamma/gamma.comp )

add_shader( single_cloth nearest_position/nearest_position.comp )

add_shader( single_cloth mix_ao/mix_ao.comp )

add_shader( single_cloth tone/tone.comp )


add_shader( single_cloth distance/distance.comp )

add_shader( single_cloth merge/merge.comp )

add_shader( single_cloth coc_gauss/horizontal.comp )
add_shader( single_cloth coc_gauss/vertical.comp )

add_shader( single_cloth sb/world.vert )
add_shader( single_cloth sb/world.geom )
add_shader( single_cloth sb/world.frag )
add_shader( single_cloth flare/world.vert )
add_shader( single_cloth flare/world.geom )
add_shader( single_cloth flare/world.frag )

add_shader( single_cloth meshlet_normal/meshlet_normal.comp )

add_shader( single_cloth attach_particle/attach_particle.comp )


