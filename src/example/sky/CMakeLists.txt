add_executable( sky
  gct.cpp
)
target_compile_definitions( sky PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( sky PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  sky
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

add_executable( sky_anim
  sky_anim.cpp
)
target_compile_definitions( sky_anim PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( sky_anim PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  sky_anim
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

add_executable( sky_anim2
  sky_anim2.cpp
)
target_compile_definitions( sky_anim2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( sky_anim2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  sky_anim2
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

add_shader( sky shadow_mat/shadow_mat.comp )

add_shader( sky shadow/world.vert )
add_shader( sky shadow/world.geom )
add_shader( sky shadow/world.frag )

add_shader( sky skyview/generate_transmittance.comp )
add_shader( sky skyview/generate_skyview.comp )
add_shader( sky skyview/generate_multiscat.comp )
add_shader( sky skyview/froxel.comp )
add_shader( sky skyview/render_froxel.comp )

add_shader( sky generate_fog/generate_fog.comp )

add_shader( sky volumetric_light/light_extinction.comp )
add_shader( sky volumetric_light/view_extinction.comp )
add_shader( sky volumetric_light/clear_scattering.comp )
add_shader( sky volumetric_light/scattering.comp )
add_shader( sky volumetric_light/render_scattering.comp )

add_shader( sky geometry/tangent.frag )
add_shader( sky geometry/tangent.geom )
add_shader( sky geometry/tangent.vert )
add_shader( sky geometry/tangent_bc.frag )
add_shader( sky geometry/tangent_bc_em.frag )
add_shader( sky geometry/tangent_bc_em_no.frag )
add_shader( sky geometry/tangent_bc_mr.frag )
add_shader( sky geometry/tangent_bc_mr_em.frag )
add_shader( sky geometry/tangent_bc_mr_em_no.frag )
add_shader( sky geometry/tangent_bc_mr_no.frag )
add_shader( sky geometry/tangent_bc_mr_oc.frag )
add_shader( sky geometry/tangent_bc_mr_oc_em.frag )
add_shader( sky geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( sky geometry/tangent_bc_mr_oc_no.frag )
add_shader( sky geometry/tangent_bc_no.frag )
add_shader( sky geometry/tangent_bc_oc.frag )
add_shader( sky geometry/tangent_bc_oc_em.frag )
add_shader( sky geometry/tangent_bc_oc_em_no.frag )
add_shader( sky geometry/tangent_bc_oc_no.frag )
add_shader( sky geometry/tangent_em.frag )
add_shader( sky geometry/tangent_em_no.frag )
add_shader( sky geometry/tangent_mr.frag )
add_shader( sky geometry/tangent_mr_em.frag )
add_shader( sky geometry/tangent_mr_em_no.frag )
add_shader( sky geometry/tangent_mr_no.frag )
add_shader( sky geometry/tangent_mr_oc.frag )
add_shader( sky geometry/tangent_mr_oc_em.frag )
add_shader( sky geometry/tangent_mr_oc_em_no.frag )
add_shader( sky geometry/tangent_mr_oc_no.frag )
add_shader( sky geometry/tangent_no.frag )
add_shader( sky geometry/tangent_oc.frag )
add_shader( sky geometry/tangent_oc_em.frag )
add_shader( sky geometry/tangent_oc_em_no.frag )
add_shader( sky geometry/tangent_oc_no.frag )
add_shader( sky geometry/world.frag )
add_shader( sky geometry/world.geom )
add_shader( sky geometry/world.vert )
add_shader( sky geometry/world_bc.frag )
add_shader( sky geometry/world_bc_em.frag )
add_shader( sky geometry/world_bc_mr.frag )
add_shader( sky geometry/world_bc_mr_em.frag )
add_shader( sky geometry/world_bc_mr_oc.frag )
add_shader( sky geometry/world_bc_mr_oc_em.frag )
add_shader( sky geometry/world_bc_oc.frag )
add_shader( sky geometry/world_bc_oc_em.frag )
add_shader( sky geometry/world_em.frag )
add_shader( sky geometry/world_mr.frag )
add_shader( sky geometry/world_mr_em.frag )
add_shader( sky geometry/world_mr_oc.frag )
add_shader( sky geometry/world_mr_oc_em.frag )
add_shader( sky geometry/world_oc.frag )
add_shader( sky geometry/world_oc_em.frag )

add_shader( sky ao/hbao_0.comp )
add_shader( sky ao/hbao_45.comp )
add_shader( sky ao/hbao_90.comp )
add_shader( sky ao/hbao_135.comp )
add_shader( sky ao/hbao.comp )

add_shader( sky gauss/h5_16.comp )
add_shader( sky gauss/v5_16.comp )
add_shader( sky gauss/h12_32.comp )
add_shader( sky gauss/v12_32.comp )

add_shader( sky selective_gauss/h5_16.comp )
add_shader( sky selective_gauss/v5_16.comp )
add_shader( sky selective_gauss/h12_32.comp )
add_shader( sky selective_gauss/v12_32.comp )

add_shader( sky lighting/lighting.comp )
add_shader( sky gamma/gamma.comp )

add_shader( sky mix_ao/mix_ao.comp )

add_shader( sky tone/tone.comp )

