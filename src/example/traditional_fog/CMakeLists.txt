add_executable( traditional_fog
  gct.cpp
)
target_compile_definitions( traditional_fog PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( traditional_fog PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  traditional_fog
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

add_shader( traditional_fog shadow_mat/shadow_mat.comp )

add_shader( traditional_fog shadow/world.vert )
add_shader( traditional_fog shadow/world.geom )
add_shader( traditional_fog shadow/world.frag )

add_shader( traditional_fog geometry/tangent.frag )
add_shader( traditional_fog geometry/tangent.geom )
add_shader( traditional_fog geometry/tangent.vert )
add_shader( traditional_fog geometry/tangent_bc.frag )
add_shader( traditional_fog geometry/tangent_bc_em.frag )
add_shader( traditional_fog geometry/tangent_bc_em_no.frag )
add_shader( traditional_fog geometry/tangent_bc_mr.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_em.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_em_no.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_no.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_oc.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_oc_em.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( traditional_fog geometry/tangent_bc_mr_oc_no.frag )
add_shader( traditional_fog geometry/tangent_bc_no.frag )
add_shader( traditional_fog geometry/tangent_bc_oc.frag )
add_shader( traditional_fog geometry/tangent_bc_oc_em.frag )
add_shader( traditional_fog geometry/tangent_bc_oc_em_no.frag )
add_shader( traditional_fog geometry/tangent_bc_oc_no.frag )
add_shader( traditional_fog geometry/tangent_em.frag )
add_shader( traditional_fog geometry/tangent_em_no.frag )
add_shader( traditional_fog geometry/tangent_mr.frag )
add_shader( traditional_fog geometry/tangent_mr_em.frag )
add_shader( traditional_fog geometry/tangent_mr_em_no.frag )
add_shader( traditional_fog geometry/tangent_mr_no.frag )
add_shader( traditional_fog geometry/tangent_mr_oc.frag )
add_shader( traditional_fog geometry/tangent_mr_oc_em.frag )
add_shader( traditional_fog geometry/tangent_mr_oc_em_no.frag )
add_shader( traditional_fog geometry/tangent_mr_oc_no.frag )
add_shader( traditional_fog geometry/tangent_no.frag )
add_shader( traditional_fog geometry/tangent_oc.frag )
add_shader( traditional_fog geometry/tangent_oc_em.frag )
add_shader( traditional_fog geometry/tangent_oc_em_no.frag )
add_shader( traditional_fog geometry/tangent_oc_no.frag )
add_shader( traditional_fog geometry/world.frag )
add_shader( traditional_fog geometry/world.geom )
add_shader( traditional_fog geometry/world.vert )
add_shader( traditional_fog geometry/world_bc.frag )
add_shader( traditional_fog geometry/world_bc_em.frag )
add_shader( traditional_fog geometry/world_bc_mr.frag )
add_shader( traditional_fog geometry/world_bc_mr_em.frag )
add_shader( traditional_fog geometry/world_bc_mr_oc.frag )
add_shader( traditional_fog geometry/world_bc_mr_oc_em.frag )
add_shader( traditional_fog geometry/world_bc_oc.frag )
add_shader( traditional_fog geometry/world_bc_oc_em.frag )
add_shader( traditional_fog geometry/world_em.frag )
add_shader( traditional_fog geometry/world_mr.frag )
add_shader( traditional_fog geometry/world_mr_em.frag )
add_shader( traditional_fog geometry/world_mr_oc.frag )
add_shader( traditional_fog geometry/world_mr_oc_em.frag )
add_shader( traditional_fog geometry/world_oc.frag )
add_shader( traditional_fog geometry/world_oc_em.frag )

add_shader( traditional_fog ao/hbao_0.comp )
add_shader( traditional_fog ao/hbao_45.comp )
add_shader( traditional_fog ao/hbao_90.comp )
add_shader( traditional_fog ao/hbao_135.comp )
add_shader( traditional_fog ao/hbao.comp )

add_shader( traditional_fog gauss/h5_16.comp )
add_shader( traditional_fog gauss/v5_16.comp )
add_shader( traditional_fog gauss/h12_32.comp )
add_shader( traditional_fog gauss/v12_32.comp )

add_shader( traditional_fog selective_gauss/h5_16.comp )
add_shader( traditional_fog selective_gauss/v5_16.comp )
add_shader( traditional_fog selective_gauss/h12_32.comp )
add_shader( traditional_fog selective_gauss/v12_32.comp )

add_shader( traditional_fog lighting/lighting.comp )
add_shader( traditional_fog gamma/gamma.comp )

add_shader( traditional_fog mix_ao/mix_ao.comp )

add_shader( traditional_fog tone/tone.comp )

