add_executable( substep_xpbd
  gct.cpp
)
target_compile_definitions( substep_xpbd PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( substep_xpbd PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  substep_xpbd
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

add_shader( substep_xpbd shadow_mat/shadow_mat.comp )

add_shader( substep_xpbd shadow/world.frag )
add_shader( substep_xpbd shadow/world.geom )
add_shader( substep_xpbd shadow/world.vert )

add_shader( substep_xpbd aabb/world.frag )
add_shader( substep_xpbd aabb/world.geom )
add_shader( substep_xpbd aabb/world.vert )

add_shader( substep_xpbd depth/world.frag )
add_shader( substep_xpbd depth/world.vert )

add_shader( substep_xpbd gauss/horizontal.comp )
add_shader( substep_xpbd gauss/vertical.comp )

add_shader( substep_xpbd lighting/lighting.comp )
add_shader( substep_xpbd gamma/gamma.comp )

add_shader( substep_xpbd nearest_position/nearest_position.comp )

add_shader( substep_xpbd mix_ao/mix_ao.comp )

add_shader( substep_xpbd tone/tone.comp )


add_shader( substep_xpbd distance/distance.comp )

add_shader( substep_xpbd merge/merge.comp )

add_shader( substep_xpbd coc_gauss/horizontal.comp )
add_shader( substep_xpbd coc_gauss/vertical.comp )

add_shader( substep_xpbd sb/world.vert )
add_shader( substep_xpbd sb/world.geom )
add_shader( substep_xpbd sb/world.frag )
add_shader( substep_xpbd flare/world.vert )
add_shader( substep_xpbd flare/world.geom )
add_shader( substep_xpbd flare/world.frag )

add_shader( substep_xpbd meshlet_normal/meshlet_normal.comp )

add_shader( substep_xpbd attach_particle/attach_particle.comp )


