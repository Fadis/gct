add_executable( froxel_sampling
  gct.cpp
)
target_compile_definitions( froxel_sampling PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( froxel_sampling PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  froxel_sampling
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

add_executable( froxel_sampling_fog
  fog.cpp
)
target_compile_definitions( froxel_sampling_fog PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( froxel_sampling_fog PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  froxel_sampling_fog
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


add_shader( froxel_sampling shadow_mat/shadow_mat.comp )

add_shader( froxel_sampling shadow/world.vert )
add_shader( froxel_sampling shadow/world.geom )
add_shader( froxel_sampling shadow/world.frag )

add_shader( froxel_sampling generate_fog/generate_fog.comp )

add_shader( froxel_sampling volumetric_light/light_extinction.comp )
add_shader( froxel_sampling volumetric_light/view_extinction.comp )
add_shader( froxel_sampling volumetric_light/clear_scattering.comp )
add_shader( froxel_sampling volumetric_light/scattering.comp )
add_shader( froxel_sampling volumetric_light/render_scattering.comp )

add_shader( froxel_sampling geometry/tangent.frag )
add_shader( froxel_sampling geometry/tangent.geom )
add_shader( froxel_sampling geometry/tangent.vert )
add_shader( froxel_sampling geometry/tangent_bc.frag )
add_shader( froxel_sampling geometry/tangent_bc_em.frag )
add_shader( froxel_sampling geometry/tangent_bc_em_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_em.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_em_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_oc.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_oc_em.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_mr_oc_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_oc.frag )
add_shader( froxel_sampling geometry/tangent_bc_oc_em.frag )
add_shader( froxel_sampling geometry/tangent_bc_oc_em_no.frag )
add_shader( froxel_sampling geometry/tangent_bc_oc_no.frag )
add_shader( froxel_sampling geometry/tangent_em.frag )
add_shader( froxel_sampling geometry/tangent_em_no.frag )
add_shader( froxel_sampling geometry/tangent_mr.frag )
add_shader( froxel_sampling geometry/tangent_mr_em.frag )
add_shader( froxel_sampling geometry/tangent_mr_em_no.frag )
add_shader( froxel_sampling geometry/tangent_mr_no.frag )
add_shader( froxel_sampling geometry/tangent_mr_oc.frag )
add_shader( froxel_sampling geometry/tangent_mr_oc_em.frag )
add_shader( froxel_sampling geometry/tangent_mr_oc_em_no.frag )
add_shader( froxel_sampling geometry/tangent_mr_oc_no.frag )
add_shader( froxel_sampling geometry/tangent_no.frag )
add_shader( froxel_sampling geometry/tangent_oc.frag )
add_shader( froxel_sampling geometry/tangent_oc_em.frag )
add_shader( froxel_sampling geometry/tangent_oc_em_no.frag )
add_shader( froxel_sampling geometry/tangent_oc_no.frag )
add_shader( froxel_sampling geometry/world.frag )
add_shader( froxel_sampling geometry/world.geom )
add_shader( froxel_sampling geometry/world.vert )
add_shader( froxel_sampling geometry/world_bc.frag )
add_shader( froxel_sampling geometry/world_bc_em.frag )
add_shader( froxel_sampling geometry/world_bc_mr.frag )
add_shader( froxel_sampling geometry/world_bc_mr_em.frag )
add_shader( froxel_sampling geometry/world_bc_mr_oc.frag )
add_shader( froxel_sampling geometry/world_bc_mr_oc_em.frag )
add_shader( froxel_sampling geometry/world_bc_oc.frag )
add_shader( froxel_sampling geometry/world_bc_oc_em.frag )
add_shader( froxel_sampling geometry/world_em.frag )
add_shader( froxel_sampling geometry/world_mr.frag )
add_shader( froxel_sampling geometry/world_mr_em.frag )
add_shader( froxel_sampling geometry/world_mr_oc.frag )
add_shader( froxel_sampling geometry/world_mr_oc_em.frag )
add_shader( froxel_sampling geometry/world_oc.frag )
add_shader( froxel_sampling geometry/world_oc_em.frag )

add_shader( froxel_sampling ao/hbao_0.comp )
add_shader( froxel_sampling ao/hbao_45.comp )
add_shader( froxel_sampling ao/hbao_90.comp )
add_shader( froxel_sampling ao/hbao_135.comp )
add_shader( froxel_sampling ao/hbao.comp )

add_shader( froxel_sampling gauss/h5_16.comp )
add_shader( froxel_sampling gauss/v5_16.comp )
add_shader( froxel_sampling gauss/h12_32.comp )
add_shader( froxel_sampling gauss/v12_32.comp )

add_shader( froxel_sampling selective_gauss/h5_16.comp )
add_shader( froxel_sampling selective_gauss/v5_16.comp )
add_shader( froxel_sampling selective_gauss/h12_32.comp )
add_shader( froxel_sampling selective_gauss/v12_32.comp )

add_shader( froxel_sampling lighting/lighting.comp )
add_shader( froxel_sampling gamma/gamma.comp )

add_shader( froxel_sampling mix_ao/mix_ao.comp )

add_shader( froxel_sampling tone/tone.comp )

