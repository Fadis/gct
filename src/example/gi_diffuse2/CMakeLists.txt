add_executable( gi_diffuse2
  gct.cpp
)
target_compile_definitions( gi_diffuse2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( gi_diffuse2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  gi_diffuse2
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

add_shader( gi_diffuse2 shadow/world.vert )
add_shader( gi_diffuse2 shadow/world.frag )

add_shader( gi_diffuse2 voxel/clear.comp )
add_shader( gi_diffuse2 voxel/world.vert )
add_shader( gi_diffuse2 voxel/world.frag )

add_shader( gi_diffuse2 voronoi/voronoi.comp )

add_shader( gi_diffuse2 distance_field/distance_field.comp )

add_shader( gi_diffuse2 geometry/tangent.frag )
add_shader( gi_diffuse2 geometry/tangent.geom )
add_shader( gi_diffuse2 geometry/tangent.vert )
add_shader( gi_diffuse2 geometry/tangent_bc.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_em.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_em.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_oc.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_oc_em.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_mr_oc_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_oc.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_oc_em.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_oc_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_bc_oc_no.frag )
add_shader( gi_diffuse2 geometry/tangent_em.frag )
add_shader( gi_diffuse2 geometry/tangent_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_mr.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_em.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_no.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_oc.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_oc_em.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_oc_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_mr_oc_no.frag )
add_shader( gi_diffuse2 geometry/tangent_no.frag )
add_shader( gi_diffuse2 geometry/tangent_oc.frag )
add_shader( gi_diffuse2 geometry/tangent_oc_em.frag )
add_shader( gi_diffuse2 geometry/tangent_oc_em_no.frag )
add_shader( gi_diffuse2 geometry/tangent_oc_no.frag )
add_shader( gi_diffuse2 geometry/world.frag )
add_shader( gi_diffuse2 geometry/world.geom )
add_shader( gi_diffuse2 geometry/world.vert )
add_shader( gi_diffuse2 geometry/world_bc.frag )
add_shader( gi_diffuse2 geometry/world_bc_em.frag )
add_shader( gi_diffuse2 geometry/world_bc_mr.frag )
add_shader( gi_diffuse2 geometry/world_bc_mr_em.frag )
add_shader( gi_diffuse2 geometry/world_bc_mr_oc.frag )
add_shader( gi_diffuse2 geometry/world_bc_mr_oc_em.frag )
add_shader( gi_diffuse2 geometry/world_bc_oc.frag )
add_shader( gi_diffuse2 geometry/world_bc_oc_em.frag )
add_shader( gi_diffuse2 geometry/world_em.frag )
add_shader( gi_diffuse2 geometry/world_mr.frag )
add_shader( gi_diffuse2 geometry/world_mr_em.frag )
add_shader( gi_diffuse2 geometry/world_mr_oc.frag )
add_shader( gi_diffuse2 geometry/world_mr_oc_em.frag )
add_shader( gi_diffuse2 geometry/world_oc.frag )
add_shader( gi_diffuse2 geometry/world_oc_em.frag )

add_shader( gi_diffuse2 ssdgi/ssdgi.comp )

add_shader( gi_diffuse2 gauss/h12_32.comp )
add_shader( gi_diffuse2 gauss/v12_32.comp )

add_shader( gi_diffuse2 selective_gauss2/h1_8.comp )
add_shader( gi_diffuse2 selective_gauss2/v1_8.comp )
add_shader( gi_diffuse2 selective_gauss2/h5_16.comp )
add_shader( gi_diffuse2 selective_gauss2/v5_16.comp )
add_shader( gi_diffuse2 selective_gauss2/h12_32.comp )
add_shader( gi_diffuse2 selective_gauss2/v12_32.comp )
add_shader( gi_diffuse2 selective_gauss2/hadaptive.comp )
add_shader( gi_diffuse2 selective_gauss2/vadaptive.comp )

add_shader( gi_diffuse2 lighting/lighting.comp )
add_shader( gi_diffuse2 gamma/gamma.comp )

add_shader( gi_diffuse2 mix_ao/mix_ao.comp )

add_shader( gi_diffuse2 tone/tone.comp )

add_shader( gi_diffuse2 reproject/reproject.comp )
add_shader( gi_diffuse2 reproject/clear.comp )
