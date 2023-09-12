add_executable( froxel_sampling_with_shadow
  gct.cpp
)
target_compile_definitions( froxel_sampling_with_shadow PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( froxel_sampling_with_shadow PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  froxel_sampling_with_shadow
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

add_shader( froxel_sampling_with_shadow shadow_mat/shadow_mat.comp )

add_shader( froxel_sampling_with_shadow shadow/world.vert )
add_shader( froxel_sampling_with_shadow shadow/world.geom )
add_shader( froxel_sampling_with_shadow shadow/world.frag )

add_shader( froxel_sampling_with_shadow generate_fog/generate_fog.comp )

add_shader( froxel_sampling_with_shadow volumetric_light/light_extinction.comp )
add_shader( froxel_sampling_with_shadow volumetric_light/view_extinction.comp )
add_shader( froxel_sampling_with_shadow volumetric_light/clear_scattering.comp )
add_shader( froxel_sampling_with_shadow volumetric_light/scattering.comp )
add_shader( froxel_sampling_with_shadow volumetric_light/render_scattering.comp )

add_shader( froxel_sampling_with_shadow geometry/tangent.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent.geom )
add_shader( froxel_sampling_with_shadow geometry/tangent.vert )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_mr_oc_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_oc_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_bc_oc_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_oc_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_mr_oc_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_oc_em_no.frag )
add_shader( froxel_sampling_with_shadow geometry/tangent_oc_no.frag )
add_shader( froxel_sampling_with_shadow geometry/world.frag )
add_shader( froxel_sampling_with_shadow geometry/world.geom )
add_shader( froxel_sampling_with_shadow geometry/world.vert )
add_shader( froxel_sampling_with_shadow geometry/world_bc.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_mr.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_mr_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_mr_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_mr_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/world_bc_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_mr.frag )
add_shader( froxel_sampling_with_shadow geometry/world_mr_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_mr_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/world_mr_oc_em.frag )
add_shader( froxel_sampling_with_shadow geometry/world_oc.frag )
add_shader( froxel_sampling_with_shadow geometry/world_oc_em.frag )

add_shader( froxel_sampling_with_shadow ao/hbao_0.comp )
add_shader( froxel_sampling_with_shadow ao/hbao_45.comp )
add_shader( froxel_sampling_with_shadow ao/hbao_90.comp )
add_shader( froxel_sampling_with_shadow ao/hbao_135.comp )
add_shader( froxel_sampling_with_shadow ao/hbao.comp )

add_shader( froxel_sampling_with_shadow gauss/h5_16.comp )
add_shader( froxel_sampling_with_shadow gauss/v5_16.comp )
add_shader( froxel_sampling_with_shadow gauss/h12_32.comp )
add_shader( froxel_sampling_with_shadow gauss/v12_32.comp )

add_shader( froxel_sampling_with_shadow selective_gauss/h5_16.comp )
add_shader( froxel_sampling_with_shadow selective_gauss/v5_16.comp )
add_shader( froxel_sampling_with_shadow selective_gauss/h12_32.comp )
add_shader( froxel_sampling_with_shadow selective_gauss/v12_32.comp )

add_shader( froxel_sampling_with_shadow lighting/lighting.comp )
add_shader( froxel_sampling_with_shadow gamma/gamma.comp )

add_shader( froxel_sampling_with_shadow mix_ao/mix_ao.comp )

add_shader( froxel_sampling_with_shadow tone/tone.comp )

