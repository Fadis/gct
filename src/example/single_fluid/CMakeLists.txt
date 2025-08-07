add_executable( single_fluid
  gct.cpp
)
target_compile_definitions( single_fluid PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( single_fluid PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  single_fluid
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

add_shader( single_fluid shadow_mat/shadow_mat.comp )

add_shader( single_fluid shadow/world.frag )
add_shader( single_fluid shadow/world.geom )
add_shader( single_fluid shadow/world.vert )

add_shader( single_fluid aabb/world.frag )
add_shader( single_fluid aabb/world.geom )
add_shader( single_fluid aabb/world.vert )

add_shader( single_fluid depth/world.frag )
add_shader( single_fluid depth/world.vert )

add_shader( single_fluid gauss/horizontal.comp )
add_shader( single_fluid gauss/vertical.comp )

add_shader( single_fluid lighting/lighting.comp )
add_shader( single_fluid gamma/gamma.comp )

add_shader( single_fluid nearest_position/nearest_position.comp )

add_shader( single_fluid mix_ao/mix_ao.comp )

add_shader( single_fluid tone/tone.comp )


add_shader( single_fluid distance/distance.comp )

add_shader( single_fluid coc_gauss/horizontal.comp )
add_shader( single_fluid coc_gauss/vertical.comp )

add_shader( single_fluid sb/world.vert )
add_shader( single_fluid sb/world.geom )
add_shader( single_fluid sb/world.frag )
add_shader( single_fluid flare/world.vert )
add_shader( single_fluid flare/world.geom )
add_shader( single_fluid flare/world.frag )

add_shader( single_fluid meshlet_normal/meshlet_normal.comp )

add_shader( single_fluid attach_particle/attach_particle.comp )


