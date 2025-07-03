add_executable( cloth
  gct.cpp
)
target_compile_definitions( cloth PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( cloth PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  cloth
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

add_shader( cloth shadow_mat/shadow_mat.comp )

add_shader( cloth shadow/world.frag )
add_shader( cloth shadow/world.geom )
add_shader( cloth shadow/world.vert )

add_shader( cloth dummy/dummy.comp )

add_shader( cloth aabb/world.frag )
add_shader( cloth aabb/world.geom )
add_shader( cloth aabb/world.vert )

add_shader( cloth depth/world.frag )
add_shader( cloth depth/world.vert )

add_shader( cloth gauss/horizontal.comp )
add_shader( cloth gauss/vertical.comp )

add_shader( cloth lighting/lighting.comp )
add_shader( cloth gamma/gamma.comp )

add_shader( cloth nearest_position/nearest_position.comp )

add_shader( cloth mix_ao/mix_ao.comp )

add_shader( cloth tone/tone.comp )


add_shader( cloth distance/distance.comp )

add_shader( cloth merge/merge.comp )

add_shader( cloth coc_gauss/horizontal.comp )
add_shader( cloth coc_gauss/vertical.comp )

add_shader( cloth sb/world.vert )
add_shader( cloth sb/world.geom )
add_shader( cloth sb/world.frag )
add_shader( cloth flare/world.vert )
add_shader( cloth flare/world.geom )
add_shader( cloth flare/world.frag )

add_shader( cloth meshlet_normal/meshlet_normal.comp )

add_shader( cloth attach_particle/attach_particle.comp )


