add_executable( fluid
  gct.cpp
)
target_compile_definitions( fluid PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( fluid PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  fluid
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

add_shader( fluid shadow_mat/shadow_mat.comp )

add_shader( fluid shadow/world.frag )
add_shader( fluid shadow/world.geom )
add_shader( fluid shadow/world.vert )

add_shader( fluid aabb/world.frag )
add_shader( fluid aabb/world.geom )
add_shader( fluid aabb/world.vert )

add_shader( fluid depth/world.frag )
add_shader( fluid depth/world.vert )

add_shader( fluid gauss/horizontal.comp )
add_shader( fluid gauss/vertical.comp )

add_shader( fluid lighting/lighting.comp )
add_shader( fluid gamma/gamma.comp )

add_shader( fluid nearest_position/nearest_position.comp )

add_shader( fluid mix_ao/mix_ao.comp )

add_shader( fluid tone/tone.comp )


add_shader( fluid distance/distance.comp )

add_shader( fluid coc_gauss/horizontal.comp )
add_shader( fluid coc_gauss/vertical.comp )

add_shader( fluid sb/world.vert )
add_shader( fluid sb/world.geom )
add_shader( fluid sb/world.frag )
add_shader( fluid flare/world.vert )
add_shader( fluid flare/world.geom )
add_shader( fluid flare/world.frag )

add_shader( fluid meshlet_normal/meshlet_normal.comp )

add_shader( fluid attach_particle/attach_particle.comp )


