add_executable( simple_particle
  gct.cpp
)
target_compile_definitions( simple_particle PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( simple_particle PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  simple_particle
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

add_shader( simple_particle shadow_mat/shadow_mat.comp )

add_shader( simple_particle shadow/world.vert )
add_shader( simple_particle shadow/world.geom )
add_shader( simple_particle shadow/world.frag )


add_shader( simple_particle particle/world.vert )
add_shader( simple_particle particle/world.geom )
add_shader( simple_particle particle/world.frag )

add_shader( simple_particle geometry/tangent.frag )
add_shader( simple_particle geometry/tangent.geom )
add_shader( simple_particle geometry/tangent.vert )
add_shader( simple_particle geometry/tangent_bc.frag )
add_shader( simple_particle geometry/tangent_bc_em.frag )
add_shader( simple_particle geometry/tangent_bc_em_no.frag )
add_shader( simple_particle geometry/tangent_bc_mr.frag )
add_shader( simple_particle geometry/tangent_bc_mr_em.frag )
add_shader( simple_particle geometry/tangent_bc_mr_em_no.frag )
add_shader( simple_particle geometry/tangent_bc_mr_no.frag )
add_shader( simple_particle geometry/tangent_bc_mr_oc.frag )
add_shader( simple_particle geometry/tangent_bc_mr_oc_em.frag )
add_shader( simple_particle geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( simple_particle geometry/tangent_bc_mr_oc_no.frag )
add_shader( simple_particle geometry/tangent_bc_no.frag )
add_shader( simple_particle geometry/tangent_bc_oc.frag )
add_shader( simple_particle geometry/tangent_bc_oc_em.frag )
add_shader( simple_particle geometry/tangent_bc_oc_em_no.frag )
add_shader( simple_particle geometry/tangent_bc_oc_no.frag )
add_shader( simple_particle geometry/tangent_em.frag )
add_shader( simple_particle geometry/tangent_em_no.frag )
add_shader( simple_particle geometry/tangent_mr.frag )
add_shader( simple_particle geometry/tangent_mr_em.frag )
add_shader( simple_particle geometry/tangent_mr_em_no.frag )
add_shader( simple_particle geometry/tangent_mr_no.frag )
add_shader( simple_particle geometry/tangent_mr_oc.frag )
add_shader( simple_particle geometry/tangent_mr_oc_em.frag )
add_shader( simple_particle geometry/tangent_mr_oc_em_no.frag )
add_shader( simple_particle geometry/tangent_mr_oc_no.frag )
add_shader( simple_particle geometry/tangent_no.frag )
add_shader( simple_particle geometry/tangent_oc.frag )
add_shader( simple_particle geometry/tangent_oc_em.frag )
add_shader( simple_particle geometry/tangent_oc_em_no.frag )
add_shader( simple_particle geometry/tangent_oc_no.frag )
add_shader( simple_particle geometry/world.frag )
add_shader( simple_particle geometry/world.geom )
add_shader( simple_particle geometry/world.vert )
add_shader( simple_particle geometry/world_bc.frag )
add_shader( simple_particle geometry/world_bc_em.frag )
add_shader( simple_particle geometry/world_bc_mr.frag )
add_shader( simple_particle geometry/world_bc_mr_em.frag )
add_shader( simple_particle geometry/world_bc_mr_oc.frag )
add_shader( simple_particle geometry/world_bc_mr_oc_em.frag )
add_shader( simple_particle geometry/world_bc_oc.frag )
add_shader( simple_particle geometry/world_bc_oc_em.frag )
add_shader( simple_particle geometry/world_em.frag )
add_shader( simple_particle geometry/world_mr.frag )
add_shader( simple_particle geometry/world_mr_em.frag )
add_shader( simple_particle geometry/world_mr_oc.frag )
add_shader( simple_particle geometry/world_mr_oc_em.frag )
add_shader( simple_particle geometry/world_oc.frag )
add_shader( simple_particle geometry/world_oc_em.frag )

add_shader( simple_particle ao/hbao_0.comp )
add_shader( simple_particle ao/hbao_45.comp )
add_shader( simple_particle ao/hbao_90.comp )
add_shader( simple_particle ao/hbao_135.comp )
add_shader( simple_particle ao/hbao.comp )

add_shader( simple_particle gauss/h5_16.comp )
add_shader( simple_particle gauss/v5_16.comp )
add_shader( simple_particle gauss/h12_32.comp )
add_shader( simple_particle gauss/v12_32.comp )

add_shader( simple_particle selective_gauss/h5_16.comp )
add_shader( simple_particle selective_gauss/v5_16.comp )
add_shader( simple_particle selective_gauss/h12_32.comp )
add_shader( simple_particle selective_gauss/v12_32.comp )

add_shader( simple_particle lighting/lighting.comp )
add_shader( simple_particle gamma/gamma.comp )

add_shader( simple_particle mix_ao/mix_ao.comp )

add_shader( simple_particle tone/tone.comp )

