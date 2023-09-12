add_executable( osm
  gct.cpp
)
target_compile_definitions( osm PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( osm PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  osm
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

add_shader( osm shadow_mat/shadow_mat.comp )

add_shader( osm shadow/world.vert )
add_shader( osm shadow/world.geom )
add_shader( osm shadow/world.frag )

add_shader( osm particle/world.vert )
add_shader( osm particle/world.geom )
add_shader( osm particle/world.frag )

add_shader( osm volumetric_light/render_scattering.comp )

add_shader( osm particle_shadow/light_extinction.vert )
add_shader( osm particle_shadow/light_extinction.geom )
add_shader( osm particle_shadow/light_extinction.frag )
add_shader( osm particle_shadow/view_extinction.vert )
add_shader( osm particle_shadow/view_extinction.geom )
add_shader( osm particle_shadow/view_extinction.frag )

add_shader( osm geometry/tangent.frag )
add_shader( osm geometry/tangent.geom )
add_shader( osm geometry/tangent.vert )
add_shader( osm geometry/tangent_bc.frag )
add_shader( osm geometry/tangent_bc_em.frag )
add_shader( osm geometry/tangent_bc_em_no.frag )
add_shader( osm geometry/tangent_bc_mr.frag )
add_shader( osm geometry/tangent_bc_mr_em.frag )
add_shader( osm geometry/tangent_bc_mr_em_no.frag )
add_shader( osm geometry/tangent_bc_mr_no.frag )
add_shader( osm geometry/tangent_bc_mr_oc.frag )
add_shader( osm geometry/tangent_bc_mr_oc_em.frag )
add_shader( osm geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( osm geometry/tangent_bc_mr_oc_no.frag )
add_shader( osm geometry/tangent_bc_no.frag )
add_shader( osm geometry/tangent_bc_oc.frag )
add_shader( osm geometry/tangent_bc_oc_em.frag )
add_shader( osm geometry/tangent_bc_oc_em_no.frag )
add_shader( osm geometry/tangent_bc_oc_no.frag )
add_shader( osm geometry/tangent_em.frag )
add_shader( osm geometry/tangent_em_no.frag )
add_shader( osm geometry/tangent_mr.frag )
add_shader( osm geometry/tangent_mr_em.frag )
add_shader( osm geometry/tangent_mr_em_no.frag )
add_shader( osm geometry/tangent_mr_no.frag )
add_shader( osm geometry/tangent_mr_oc.frag )
add_shader( osm geometry/tangent_mr_oc_em.frag )
add_shader( osm geometry/tangent_mr_oc_em_no.frag )
add_shader( osm geometry/tangent_mr_oc_no.frag )
add_shader( osm geometry/tangent_no.frag )
add_shader( osm geometry/tangent_oc.frag )
add_shader( osm geometry/tangent_oc_em.frag )
add_shader( osm geometry/tangent_oc_em_no.frag )
add_shader( osm geometry/tangent_oc_no.frag )
add_shader( osm geometry/world.frag )
add_shader( osm geometry/world.geom )
add_shader( osm geometry/world.vert )
add_shader( osm geometry/world_bc.frag )
add_shader( osm geometry/world_bc_em.frag )
add_shader( osm geometry/world_bc_mr.frag )
add_shader( osm geometry/world_bc_mr_em.frag )
add_shader( osm geometry/world_bc_mr_oc.frag )
add_shader( osm geometry/world_bc_mr_oc_em.frag )
add_shader( osm geometry/world_bc_oc.frag )
add_shader( osm geometry/world_bc_oc_em.frag )
add_shader( osm geometry/world_em.frag )
add_shader( osm geometry/world_mr.frag )
add_shader( osm geometry/world_mr_em.frag )
add_shader( osm geometry/world_mr_oc.frag )
add_shader( osm geometry/world_mr_oc_em.frag )
add_shader( osm geometry/world_oc.frag )
add_shader( osm geometry/world_oc_em.frag )

add_shader( osm ao/hbao_0.comp )
add_shader( osm ao/hbao_45.comp )
add_shader( osm ao/hbao_90.comp )
add_shader( osm ao/hbao_135.comp )
add_shader( osm ao/hbao.comp )

add_shader( osm gauss/h5_16.comp )
add_shader( osm gauss/v5_16.comp )
add_shader( osm gauss/h12_32.comp )
add_shader( osm gauss/v12_32.comp )

add_shader( osm selective_gauss/h5_16.comp )
add_shader( osm selective_gauss/v5_16.comp )
add_shader( osm selective_gauss/h12_32.comp )
add_shader( osm selective_gauss/v12_32.comp )

add_shader( osm lighting/lighting.comp )
add_shader( osm gamma/gamma.comp )

add_shader( osm mix_ao/mix_ao.comp )

add_shader( osm tone/tone.comp )

