add_executable( standard
  gct.cpp
)
target_compile_definitions( standard PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( standard PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  standard
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

add_shader( standard aabb/world.frag )
add_shader( standard aabb/world.geom )
add_shader( standard aabb/world.vert )

add_shader( standard depth/world.frag )
add_shader( standard depth/world.vert )

add_shader( standard gauss/horizontal.comp )
add_shader( standard gauss/vertical.comp )

add_shader( standard lighting/lighting.comp )
add_shader( standard gamma/gamma.comp )

add_shader( standard nearest_position/nearest_position.comp )

add_shader( standard mix_ao/mix_ao.comp )

add_shader( standard tone/tone.comp )


add_shader( standard distance/distance.comp )

add_shader( standard merge/merge.comp )

add_shader( standard coc_gauss/horizontal.comp )
add_shader( standard coc_gauss/vertical.comp )

add_shader( standard sb/world.vert )
add_shader( standard sb/world.geom )
add_shader( standard sb/world.frag )
add_shader( standard flare/world.vert )
add_shader( standard flare/world.geom )
add_shader( standard flare/world.frag )

add_shader( standard meshlet_normal/meshlet_normal.comp )

