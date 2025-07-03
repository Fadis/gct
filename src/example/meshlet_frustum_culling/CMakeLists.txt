add_executable( meshlet_frustum_culling
  gct.cpp
)
target_compile_definitions( meshlet_frustum_culling PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( meshlet_frustum_culling PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  meshlet_frustum_culling
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

add_shader( meshlet_frustum_culling shadow_mat/shadow_mat.comp )

add_shader( meshlet_frustum_culling shadow/world.frag )
add_shader( meshlet_frustum_culling shadow/world.geom )
add_shader( meshlet_frustum_culling shadow/world.vert )

add_shader( meshlet_frustum_culling geometry/geometry.task )
add_shader( meshlet_frustum_culling geometry/geometry.mesh )
add_shader( meshlet_frustum_culling geometry/geometry.frag )
add_shader( meshlet_frustum_culling geometry/dummy.comp )

add_shader( meshlet_frustum_culling aabb/world.frag )
add_shader( meshlet_frustum_culling aabb/world.geom )
add_shader( meshlet_frustum_culling aabb/world.vert )

add_shader( meshlet_frustum_culling depth/world.frag )
add_shader( meshlet_frustum_culling depth/world.vert )

add_shader( meshlet_frustum_culling gauss/horizontal.comp )
add_shader( meshlet_frustum_culling gauss/vertical.comp )

add_shader( meshlet_frustum_culling lighting/lighting.comp )
add_shader( meshlet_frustum_culling gamma/gamma.comp )

add_shader( meshlet_frustum_culling nearest_position/nearest_position.comp )

add_shader( meshlet_frustum_culling mix_ao/mix_ao.comp )

add_shader( meshlet_frustum_culling tone/tone.comp )


add_shader( meshlet_frustum_culling distance/distance.comp )

add_shader( meshlet_frustum_culling merge/merge.comp )

add_shader( meshlet_frustum_culling coc_gauss/horizontal.comp )
add_shader( meshlet_frustum_culling coc_gauss/vertical.comp )

add_shader( meshlet_frustum_culling sb/world.vert )
add_shader( meshlet_frustum_culling sb/world.geom )
add_shader( meshlet_frustum_culling sb/world.frag )
add_shader( meshlet_frustum_culling flare/world.vert )
add_shader( meshlet_frustum_culling flare/world.geom )
add_shader( meshlet_frustum_culling flare/world.frag )

add_shader( meshlet_frustum_culling meshlet_normal/meshlet_normal.comp )

