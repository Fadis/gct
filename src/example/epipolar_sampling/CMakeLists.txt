add_executable( epipolar_sampling
  gct.cpp
)
target_compile_definitions( epipolar_sampling PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( epipolar_sampling PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  epipolar_sampling
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

add_shader( epipolar_sampling shadow_mat/shadow_mat.comp )

add_shader( epipolar_sampling shadow/world.vert )
add_shader( epipolar_sampling shadow/world.geom )
add_shader( epipolar_sampling shadow/world.frag )

add_shader( epipolar_sampling epipolar/generate2.comp )
add_shader( epipolar_sampling epipolar/generate3.comp )
add_shader( epipolar_sampling epipolar/generate4.comp )
add_shader( epipolar_sampling epipolar/world.vert )
add_shader( epipolar_sampling epipolar/world.frag )

add_shader( epipolar_sampling volumetric_light/volumetric_light.comp )
add_shader( epipolar_sampling volumetric_light/texcoord.comp )
add_shader( epipolar_sampling volumetric_light/clear_texcoord.comp )
add_shader( epipolar_sampling volumetric_light/fill_texcoord.comp )

add_shader( epipolar_sampling geometry/tangent.frag )
add_shader( epipolar_sampling geometry/tangent.geom )
add_shader( epipolar_sampling geometry/tangent.vert )
add_shader( epipolar_sampling geometry/tangent_bc.frag )
add_shader( epipolar_sampling geometry/tangent_bc_em.frag )
add_shader( epipolar_sampling geometry/tangent_bc_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_em.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_oc.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_oc_em.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_mr_oc_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_oc.frag )
add_shader( epipolar_sampling geometry/tangent_bc_oc_em.frag )
add_shader( epipolar_sampling geometry/tangent_bc_oc_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_bc_oc_no.frag )
add_shader( epipolar_sampling geometry/tangent_em.frag )
add_shader( epipolar_sampling geometry/tangent_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_mr.frag )
add_shader( epipolar_sampling geometry/tangent_mr_em.frag )
add_shader( epipolar_sampling geometry/tangent_mr_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_mr_no.frag )
add_shader( epipolar_sampling geometry/tangent_mr_oc.frag )
add_shader( epipolar_sampling geometry/tangent_mr_oc_em.frag )
add_shader( epipolar_sampling geometry/tangent_mr_oc_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_mr_oc_no.frag )
add_shader( epipolar_sampling geometry/tangent_no.frag )
add_shader( epipolar_sampling geometry/tangent_oc.frag )
add_shader( epipolar_sampling geometry/tangent_oc_em.frag )
add_shader( epipolar_sampling geometry/tangent_oc_em_no.frag )
add_shader( epipolar_sampling geometry/tangent_oc_no.frag )
add_shader( epipolar_sampling geometry/world.frag )
add_shader( epipolar_sampling geometry/world.geom )
add_shader( epipolar_sampling geometry/world.vert )
add_shader( epipolar_sampling geometry/world_bc.frag )
add_shader( epipolar_sampling geometry/world_bc_em.frag )
add_shader( epipolar_sampling geometry/world_bc_mr.frag )
add_shader( epipolar_sampling geometry/world_bc_mr_em.frag )
add_shader( epipolar_sampling geometry/world_bc_mr_oc.frag )
add_shader( epipolar_sampling geometry/world_bc_mr_oc_em.frag )
add_shader( epipolar_sampling geometry/world_bc_oc.frag )
add_shader( epipolar_sampling geometry/world_bc_oc_em.frag )
add_shader( epipolar_sampling geometry/world_em.frag )
add_shader( epipolar_sampling geometry/world_mr.frag )
add_shader( epipolar_sampling geometry/world_mr_em.frag )
add_shader( epipolar_sampling geometry/world_mr_oc.frag )
add_shader( epipolar_sampling geometry/world_mr_oc_em.frag )
add_shader( epipolar_sampling geometry/world_oc.frag )
add_shader( epipolar_sampling geometry/world_oc_em.frag )

add_shader( epipolar_sampling ao/hbao_0.comp )
add_shader( epipolar_sampling ao/hbao_45.comp )
add_shader( epipolar_sampling ao/hbao_90.comp )
add_shader( epipolar_sampling ao/hbao_135.comp )
add_shader( epipolar_sampling ao/hbao.comp )

add_shader( epipolar_sampling gauss/h5_16.comp )
add_shader( epipolar_sampling gauss/v5_16.comp )
add_shader( epipolar_sampling gauss/h12_32.comp )
add_shader( epipolar_sampling gauss/v12_32.comp )

add_shader( epipolar_sampling selective_gauss/h5_16.comp )
add_shader( epipolar_sampling selective_gauss/v5_16.comp )
add_shader( epipolar_sampling selective_gauss/h12_32.comp )
add_shader( epipolar_sampling selective_gauss/v12_32.comp )

add_shader( epipolar_sampling lighting/lighting.comp )
add_shader( epipolar_sampling gamma/gamma.comp )

add_shader( epipolar_sampling mix_ao/mix_ao.comp )

add_shader( epipolar_sampling tone/tone.comp )

