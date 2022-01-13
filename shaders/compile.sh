GLSLC=glslc 
GLSLI=../build/src/glsl_include

echo simple.rgen
cat simple.rgen|${GLSLI}|${GLSLC} -fshader-stage=rgen -o simple.rgen.spv --target-env=vulkan1.2 -
echo simple.rchit
cat simple.rchit|${GLSLI}|${GLSLC} -fshader-stage=rchit -o simple.rchit.spv --target-env=vulkan1.2 -
echo simple.rmiss
cat simple.rmiss|${GLSLI}|${GLSLC} -fshader-stage=rmiss -o simple.rmiss.spv --target-env=vulkan1.2 -
echo simple2.rmiss
cat simple2.rmiss|${GLSLI}|${GLSLC} -fshader-stage=rmiss -o simple2.rmiss.spv --target-env=vulkan1.2 -

echo world.vert
cat world.vert|${GLSLI}|${GLSLC} -fshader-stage=vert -o world.vert.spv --target-env=vulkan1.2 -
echo tangent.vert
cat tangent.vert|${GLSLI}|${GLSLC} -fshader-stage=vert -o tangent.vert.spv --target-env=vulkan1.2 -


echo world.frag
cat world.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world.frag.spv --target-env=vulkan1.2 -
echo world_bc.frag
cat world_bc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc.frag.spv --target-env=vulkan1.2 -
echo world_mr.frag
cat world_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr.frag
cat world_bc_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_oc.frag
cat world_bc_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc.frag.spv --target-env=vulkan1.2 -
echo world_bc_oc.frag
cat world_bc_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc.frag.spv --target-env=vulkan1.2 -
echo world_mr_oc.frag
cat world_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc.frag.spv --target-env=vulkan1.2 -
echo world_oc.frag
cat world_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc.frag.spv --target-env=vulkan1.2 -

echo world_em.frag
cat world_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_em.frag.spv --target-env=vulkan1.2 -
echo world_bc_em.frag
cat world_bc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_em.frag.spv --target-env=vulkan1.2 -
echo world_mr_em.frag
cat world_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_em.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_em.frag
cat world_bc_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_em.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_oc_em.frag
cat world_bc_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc_em.frag.spv --target-env=vulkan1.2 -
echo world_bc_oc_em.frag
cat world_bc_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc_em.frag.spv --target-env=vulkan1.2 -
echo world_mr_oc_em.frag
cat world_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc_em.frag.spv --target-env=vulkan1.2 -
echo world_oc_em.frag
cat world_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc_em.frag.spv --target-env=vulkan1.2 -


echo tangent.frag
cat tangent.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent.frag.spv --target-env=vulkan1.2 -
echo tangent_bc.frag
cat tangent_bc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc.frag.spv --target-env=vulkan1.2 -
echo tangent_mr.frag
cat tangent_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr.frag
cat tangent_bc_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc.frag
cat tangent_bc_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc.frag
cat tangent_bc_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc.frag
cat tangent_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc.frag.spv --target-env=vulkan1.2 -
echo tangent_oc.frag
cat tangent_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc.frag.spv --target-env=vulkan1.2 -

echo tangent_em.frag
cat tangent_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_em.frag
cat tangent_bc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_em.frag
cat tangent_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_em.frag
cat tangent_bc_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_em.frag
cat tangent_bc_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_em.frag
cat tangent_bc_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_em.frag
cat tangent_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_em.frag
cat tangent_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em.frag.spv --target-env=vulkan1.2 -


echo tangent_no.frag
cat tangent_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_no.frag
cat tangent_bc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_no.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_no.frag
cat tangent_mr_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_no.frag
cat tangent_bc_mr_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_no.frag
cat tangent_bc_mr_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_no.frag
cat tangent_bc_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_no.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_no.frag
cat tangent_mr_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_no.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_no.frag
cat tangent_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_no.frag.spv --target-env=vulkan1.2 -

echo tangent_em_no.frag
cat tangent_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_em_no.frag
cat tangent_bc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_em_no.frag
cat tangent_mr_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_em_no.frag
cat tangent_bc_mr_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_em_no.frag
cat tangent_bc_mr_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_em_no.frag
cat tangent_bc_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_em_no.frag
cat tangent_mr_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em_no.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_em_no.frag
cat tangent_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em_no.frag.spv --target-env=vulkan1.2 -

echo world_sh.frag
cat world_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_sh.frag
cat world_bc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_sh.frag.spv --target-env=vulkan1.2 -
echo world_mr_sh.frag
cat world_mr_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_sh.frag
cat world_bc_mr_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_oc_sh.frag
cat world_bc_mr_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_oc_sh.frag
cat world_bc_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc_sh.frag.spv --target-env=vulkan1.2 -
echo world_mr_oc_sh.frag
cat world_mr_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc_sh.frag.spv --target-env=vulkan1.2 -
echo world_oc_sh.frag
cat world_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc_sh.frag.spv --target-env=vulkan1.2 -

echo world_em_sh.frag
cat world_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_em_sh.frag
cat world_bc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_mr_em_sh.frag
cat world_mr_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_em_sh.frag
cat world_bc_mr_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_bc_mr_oc_em_sh.frag
cat world_bc_mr_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_mc_oc_em_sh.frag
cat world_bc_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_mr_oc_em_sh.frag
cat world_mr_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo world_oc_em_sh.frag
cat world_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc_em_sh.frag.spv --target-env=vulkan1.2 -


echo tangent_sh.frag
cat tangent_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_sh.frag
cat tangent_bc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_sh.frag
cat tangent_mr_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_sh.frag
cat tangent_bc_mr_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_sh.frag
cat tangent_bc_mr_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_sh.frag
cat tangent_bc_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_sh.frag
cat tangent_mr_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_sh.frag
cat tangent_oc_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_sh.frag.spv --target-env=vulkan1.2 -

echo tangent_em_sh.frag
cat tangent_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_em_sh.frag
cat tangent_bc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_em_sh.frag
cat tangent_mr_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_em_sh.frag
cat tangent_bc_mr_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_em_sh.frag
cat tangent_bc_mr_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_em_sh.frag
cat tangent_bc_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_em_sh.frag
cat tangent_mr_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_em_sh.frag
cat tangent_oc_em_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em_sh.frag.spv --target-env=vulkan1.2 -


echo tangent_no_sh.frag
cat tangent_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_no_sh.frag
cat tangent_bc_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_no_sh.frag
cat tangent_mr_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_no_sh.frag
cat tangent_bc_mr_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_no_sh.frag
cat tangent_bc_mr_oc_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_no_sh.frag
cat tangent_bc_oc_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_no_sh.frag
cat tangent_mr_oc_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_no_sh.frag
cat tangent_oc_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_no_sh.frag.spv --target-env=vulkan1.2 -

echo tangent_em_no_sh.frag
cat tangent_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_em_no_sh.frag
cat tangent_bc_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_em_no_sh.frag
cat tangent_mr_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_em_no_sh.frag
cat tangent_bc_mr_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_mr_oc_em_no_sh.frag
cat tangent_bc_mr_oc_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_bc_oc_em_no_sh.frag
cat tangent_bc_oc_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_mr_oc_em_no_sh.frag
cat tangent_mr_oc_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em_no_sh.frag.spv --target-env=vulkan1.2 -
echo tangent_oc_em_no_sh.frag
cat tangent_oc_em_no_sh.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em_no_sh.frag.spv --target-env=vulkan1.2 -



echo special0.frag
cat special0.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special0.frag.spv --target-env=vulkan1.2 -
echo special1.frag
cat special1.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special1.frag.spv --target-env=vulkan1.2 -
echo special2.frag
cat special2.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special2.frag.spv --target-env=vulkan1.2 -
echo special3.frag
cat special3.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special3.frag.spv --target-env=vulkan1.2 -
echo special4.frag
cat special4.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special4.frag.spv --target-env=vulkan1.2 -
echo special5.vert
cat special5.vert|${GLSLI}|${GLSLC} -fshader-stage=vert -o special5.vert.spv --target-env=vulkan1.2 -

echo add.comp
cat add.comp|${GLSLI}|${GLSLC} -fshader-stage=comp -o add.comp.spv --target-env=vulkan1.2 -
echo invert.comp
cat invert.comp|${GLSLI}|${GLSLC} -fshader-stage=comp -o invert.comp.spv --target-env=vulkan1.2 -
echo grad.comp
cat grad.comp|${GLSLI}|${GLSLC} -fshader-stage=comp -o grad.comp.spv --target-env=vulkan1.2 -
