add_executable( xpbd
  gct.cpp
)
target_compile_definitions( xpbd PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( xpbd PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  xpbd
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

add_shader( xpbd shadow_mat/shadow_mat.comp )

add_shader( xpbd shadow/world.frag )
add_shader( xpbd shadow/world.geom )
add_shader( xpbd shadow/world.vert )

add_shader( xpbd aabb/world.frag )
add_shader( xpbd aabb/world.geom )
add_shader( xpbd aabb/world.vert )

add_shader( xpbd depth/world.frag )
add_shader( xpbd depth/world.vert )

add_shader( xpbd gauss/horizontal.comp )
add_shader( xpbd gauss/vertical.comp )

add_shader( xpbd lighting/lighting.comp )
add_shader( xpbd gamma/gamma.comp )

add_shader( xpbd nearest_position/nearest_position.comp )

add_shader( xpbd mix_ao/mix_ao.comp )

add_shader( xpbd tone/tone.comp )


add_shader( xpbd distance/distance.comp )

add_shader( xpbd merge/merge.comp )

add_shader( xpbd coc_gauss/horizontal.comp )
add_shader( xpbd coc_gauss/vertical.comp )

add_shader( xpbd sb/world.vert )
add_shader( xpbd sb/world.geom )
add_shader( xpbd sb/world.frag )
add_shader( xpbd flare/world.vert )
add_shader( xpbd flare/world.geom )
add_shader( xpbd flare/world.frag )

add_shader( xpbd meshlet_normal/meshlet_normal.comp )

add_shader( xpbd attach_particle/attach_particle.comp )


