add_executable( transparent_object
  gct.cpp
)
target_compile_definitions( transparent_object PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( transparent_object PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  transparent_object
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

add_shader( transparent_object shadow_mat/shadow_mat.comp )

add_shader( transparent_object shadow/world.vert )
add_shader( transparent_object shadow/world.geom )
add_shader( transparent_object shadow/world.frag )

add_shader( transparent_object volumetric_light/volumetric_light.comp )

add_shader( transparent_object geometry/tangent.frag )
add_shader( transparent_object geometry/tangent.geom )
add_shader( transparent_object geometry/tangent.vert )
add_shader( transparent_object geometry/tangent_bc.frag )
add_shader( transparent_object geometry/tangent_bc_em.frag )
add_shader( transparent_object geometry/tangent_bc_em_no.frag )
add_shader( transparent_object geometry/tangent_bc_mr.frag )
add_shader( transparent_object geometry/tangent_bc_mr_em.frag )
add_shader( transparent_object geometry/tangent_bc_mr_em_no.frag )
add_shader( transparent_object geometry/tangent_bc_mr_no.frag )
add_shader( transparent_object geometry/tangent_bc_mr_oc.frag )
add_shader( transparent_object geometry/tangent_bc_mr_oc_em.frag )
add_shader( transparent_object geometry/tangent_bc_mr_oc_em_no.frag )
add_shader( transparent_object geometry/tangent_bc_mr_oc_no.frag )
add_shader( transparent_object geometry/tangent_bc_no.frag )
add_shader( transparent_object geometry/tangent_bc_oc.frag )
add_shader( transparent_object geometry/tangent_bc_oc_em.frag )
add_shader( transparent_object geometry/tangent_bc_oc_em_no.frag )
add_shader( transparent_object geometry/tangent_bc_oc_no.frag )
add_shader( transparent_object geometry/tangent_em.frag )
add_shader( transparent_object geometry/tangent_em_no.frag )
add_shader( transparent_object geometry/tangent_mr.frag )
add_shader( transparent_object geometry/tangent_mr_em.frag )
add_shader( transparent_object geometry/tangent_mr_em_no.frag )
add_shader( transparent_object geometry/tangent_mr_no.frag )
add_shader( transparent_object geometry/tangent_mr_oc.frag )
add_shader( transparent_object geometry/tangent_mr_oc_em.frag )
add_shader( transparent_object geometry/tangent_mr_oc_em_no.frag )
add_shader( transparent_object geometry/tangent_mr_oc_no.frag )
add_shader( transparent_object geometry/tangent_no.frag )
add_shader( transparent_object geometry/tangent_oc.frag )
add_shader( transparent_object geometry/tangent_oc_em.frag )
add_shader( transparent_object geometry/tangent_oc_em_no.frag )
add_shader( transparent_object geometry/tangent_oc_no.frag )
add_shader( transparent_object geometry/world.frag )
add_shader( transparent_object geometry/world.geom )
add_shader( transparent_object geometry/world.vert )
add_shader( transparent_object geometry/world_bc.frag )
add_shader( transparent_object geometry/world_bc_em.frag )
add_shader( transparent_object geometry/world_bc_mr.frag )
add_shader( transparent_object geometry/world_bc_mr_em.frag )
add_shader( transparent_object geometry/world_bc_mr_oc.frag )
add_shader( transparent_object geometry/world_bc_mr_oc_em.frag )
add_shader( transparent_object geometry/world_bc_oc.frag )
add_shader( transparent_object geometry/world_bc_oc_em.frag )
add_shader( transparent_object geometry/world_em.frag )
add_shader( transparent_object geometry/world_mr.frag )
add_shader( transparent_object geometry/world_mr_em.frag )
add_shader( transparent_object geometry/world_mr_oc.frag )
add_shader( transparent_object geometry/world_mr_oc_em.frag )
add_shader( transparent_object geometry/world_oc.frag )
add_shader( transparent_object geometry/world_oc_em.frag )

add_shader( transparent_object depth_only/world.vert )
add_shader( transparent_object depth_only/world.frag )

add_shader( transparent_object ao/hbao_0.comp )
add_shader( transparent_object ao/hbao_45.comp )
add_shader( transparent_object ao/hbao_90.comp )
add_shader( transparent_object ao/hbao_135.comp )
add_shader( transparent_object ao/hbao.comp )

add_shader( transparent_object gauss/h5_16.comp )
add_shader( transparent_object gauss/v5_16.comp )
add_shader( transparent_object gauss/h12_32.comp )
add_shader( transparent_object gauss/v12_32.comp )

add_shader( transparent_object selective_gauss/h5_16.comp )
add_shader( transparent_object selective_gauss/v5_16.comp )
add_shader( transparent_object selective_gauss/h12_32.comp )
add_shader( transparent_object selective_gauss/v12_32.comp )

add_shader( transparent_object lighting/lighting.comp )
add_shader( transparent_object transparent_lighting/lighting.comp )
add_shader( transparent_object gamma/gamma.comp )

add_shader( transparent_object mix_ao/mix_ao.comp )

add_shader( transparent_object tone/tone.comp )

