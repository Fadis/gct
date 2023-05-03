add_executable( rtao
  gct.cpp
)
target_compile_definitions( rtao PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( rtao PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  rtao
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

add_shader( rtao shadow/world.vert )
add_shader( rtao shadow/world.frag )

add_shader( rtao voxel/clear.comp )
add_shader( rtao voxel/world.vert )
add_shader( rtao voxel/world.frag )

add_shader( rtao voronoi/voronoi.comp )

add_shader( rtao distance_field/distance_field.comp )

add_shader( rtao geometry/tangent.frag )
add_shader( rtao geometry/tangent.geom )
add_shader( rtao geometry/tangent.vert )
add_shader( rtao geometry/tangent_bc.frag )
add_shader( rtao geometry/tangent_bc_em.frag )
add_shader( rtao geometry/tangent_bc_em_no.frag )
add_shader( rtao geometry/tangent_bc_mr.frag )
add_shader( rtao geometry/tangent_bc_mr_em.frag )
add_shader( rtao geometry/tangent_bc_mr_em_no.frag )
add_shader( rtao geometry/tangent_bc_mr_no.frag )
add_shader( rtao geometry/tangent_bc_mr_oc.frag )
add_shader( rtao geometry/tangent_bc_mr_oc_em.frag )
add_shader( rtao geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( rtao geometry/tangent_bc_mr_oc_no.frag )
add_shader( rtao geometry/tangent_bc_no.frag )
add_shader( rtao geometry/tangent_bc_oc.frag )
add_shader( rtao geometry/tangent_bc_oc_em.frag )
add_shader( rtao geometry/tangent_bc_oc_em_no.frag )
add_shader( rtao geometry/tangent_bc_oc_no.frag )
add_shader( rtao geometry/tangent_em.frag )
add_shader( rtao geometry/tangent_em_no.frag )
add_shader( rtao geometry/tangent_mr.frag )
add_shader( rtao geometry/tangent_mr_em.frag )
add_shader( rtao geometry/tangent_mr_em_no.frag )
add_shader( rtao geometry/tangent_mr_no.frag )
add_shader( rtao geometry/tangent_mr_oc.frag )
add_shader( rtao geometry/tangent_mr_oc_em.frag )
add_shader( rtao geometry/tangent_mr_oc_em_no.frag )
add_shader( rtao geometry/tangent_mr_oc_no.frag )
add_shader( rtao geometry/tangent_no.frag )
add_shader( rtao geometry/tangent_oc.frag )
add_shader( rtao geometry/tangent_oc_em.frag )
add_shader( rtao geometry/tangent_oc_em_no.frag )
add_shader( rtao geometry/tangent_oc_no.frag )
add_shader( rtao geometry/world.frag )
add_shader( rtao geometry/world.geom )
add_shader( rtao geometry/world.vert )
add_shader( rtao geometry/world_bc.frag )
add_shader( rtao geometry/world_bc_em.frag )
add_shader( rtao geometry/world_bc_mr.frag )
add_shader( rtao geometry/world_bc_mr_em.frag )
add_shader( rtao geometry/world_bc_mr_oc.frag )
add_shader( rtao geometry/world_bc_mr_oc_em.frag )
add_shader( rtao geometry/world_bc_oc.frag )
add_shader( rtao geometry/world_bc_oc_em.frag )
add_shader( rtao geometry/world_em.frag )
add_shader( rtao geometry/world_mr.frag )
add_shader( rtao geometry/world_mr_em.frag )
add_shader( rtao geometry/world_mr_oc.frag )
add_shader( rtao geometry/world_mr_oc_em.frag )
add_shader( rtao geometry/world_oc.frag )
add_shader( rtao geometry/world_oc_em.frag )

add_shader( rtao ao/rtao.comp )

add_shader( rtao gauss/h12_32.comp )
add_shader( rtao gauss/v12_32.comp )

add_shader( rtao selective_gauss2/h1_8.comp )
add_shader( rtao selective_gauss2/v1_8.comp )
add_shader( rtao selective_gauss2/h12_32.comp )
add_shader( rtao selective_gauss2/v12_32.comp )
add_shader( rtao selective_gauss2/hadaptive.comp )
add_shader( rtao selective_gauss2/vadaptive.comp )

add_shader( rtao lighting/lighting.comp )
add_shader( rtao gamma/gamma.comp )

add_shader( rtao mix_ao/mix_ao.comp )

add_shader( rtao tone/tone.comp )

add_shader( rtao reproject/reproject.comp )
add_shader( rtao reproject/clear.comp )
