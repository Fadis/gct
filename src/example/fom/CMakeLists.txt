add_executable( fom
  gct.cpp
)
target_compile_definitions( fom PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( fom PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  fom
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

add_shader( fom shadow_mat/shadow_mat.comp )

add_shader( fom shadow/world.vert )
add_shader( fom shadow/world.geom )
add_shader( fom shadow/world.frag )

add_shader( fom particle/world.vert )
add_shader( fom particle/world.geom )
add_shader( fom particle/world.frag )

add_shader( fom volumetric_light/render_scattering.comp )

add_shader( fom particle_shadow/light_extinction.vert )
add_shader( fom particle_shadow/light_extinction.geom )
add_shader( fom particle_shadow/light_extinction.frag )
add_shader( fom particle_shadow/view_extinction.vert )
add_shader( fom particle_shadow/view_extinction.geom )
add_shader( fom particle_shadow/view_extinction.frag )

add_shader( fom geometry/tangent.frag )
add_shader( fom geometry/tangent.geom )
add_shader( fom geometry/tangent.vert )
add_shader( fom geometry/tangent_bc.frag )
add_shader( fom geometry/tangent_bc_em.frag )
add_shader( fom geometry/tangent_bc_em_no.frag )
add_shader( fom geometry/tangent_bc_mr.frag )
add_shader( fom geometry/tangent_bc_mr_em.frag )
add_shader( fom geometry/tangent_bc_mr_em_no.frag )
add_shader( fom geometry/tangent_bc_mr_no.frag )
add_shader( fom geometry/tangent_bc_mr_oc.frag )
add_shader( fom geometry/tangent_bc_mr_oc_em.frag )
add_shader( fom geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( fom geometry/tangent_bc_mr_oc_no.frag )
add_shader( fom geometry/tangent_bc_no.frag )
add_shader( fom geometry/tangent_bc_oc.frag )
add_shader( fom geometry/tangent_bc_oc_em.frag )
add_shader( fom geometry/tangent_bc_oc_em_no.frag )
add_shader( fom geometry/tangent_bc_oc_no.frag )
add_shader( fom geometry/tangent_em.frag )
add_shader( fom geometry/tangent_em_no.frag )
add_shader( fom geometry/tangent_mr.frag )
add_shader( fom geometry/tangent_mr_em.frag )
add_shader( fom geometry/tangent_mr_em_no.frag )
add_shader( fom geometry/tangent_mr_no.frag )
add_shader( fom geometry/tangent_mr_oc.frag )
add_shader( fom geometry/tangent_mr_oc_em.frag )
add_shader( fom geometry/tangent_mr_oc_em_no.frag )
add_shader( fom geometry/tangent_mr_oc_no.frag )
add_shader( fom geometry/tangent_no.frag )
add_shader( fom geometry/tangent_oc.frag )
add_shader( fom geometry/tangent_oc_em.frag )
add_shader( fom geometry/tangent_oc_em_no.frag )
add_shader( fom geometry/tangent_oc_no.frag )
add_shader( fom geometry/world.frag )
add_shader( fom geometry/world.geom )
add_shader( fom geometry/world.vert )
add_shader( fom geometry/world_bc.frag )
add_shader( fom geometry/world_bc_em.frag )
add_shader( fom geometry/world_bc_mr.frag )
add_shader( fom geometry/world_bc_mr_em.frag )
add_shader( fom geometry/world_bc_mr_oc.frag )
add_shader( fom geometry/world_bc_mr_oc_em.frag )
add_shader( fom geometry/world_bc_oc.frag )
add_shader( fom geometry/world_bc_oc_em.frag )
add_shader( fom geometry/world_em.frag )
add_shader( fom geometry/world_mr.frag )
add_shader( fom geometry/world_mr_em.frag )
add_shader( fom geometry/world_mr_oc.frag )
add_shader( fom geometry/world_mr_oc_em.frag )
add_shader( fom geometry/world_oc.frag )
add_shader( fom geometry/world_oc_em.frag )

add_shader( fom ao/hbao_0.comp )
add_shader( fom ao/hbao_45.comp )
add_shader( fom ao/hbao_90.comp )
add_shader( fom ao/hbao_135.comp )
add_shader( fom ao/hbao.comp )

add_shader( fom gauss/h5_16.comp )
add_shader( fom gauss/v5_16.comp )
add_shader( fom gauss/h12_32.comp )
add_shader( fom gauss/v12_32.comp )

add_shader( fom selective_gauss/h5_16.comp )
add_shader( fom selective_gauss/v5_16.comp )
add_shader( fom selective_gauss/h12_32.comp )
add_shader( fom selective_gauss/v12_32.comp )

add_shader( fom lighting/lighting.comp )
add_shader( fom gamma/gamma.comp )

add_shader( fom mix_ao/mix_ao.comp )

add_shader( fom tone/tone.comp )

