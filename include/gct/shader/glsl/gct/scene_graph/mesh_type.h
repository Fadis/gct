#ifndef GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H

struct mesh_type {
  uint position_id;
  uint normal_id;
  uint tangent_id;
  uint reserved;
  uint texcoord_0_id;
  uint texcoord_1_id;
  uint texcoord_2_id;
  uint texcoord_3_id;
  uint color_0_id;
  uint color_1_id;
  uint color_2_id;
  uint color_3_id;
  uint joint_0_id;
  uint joint_1_id;
  uint joint_2_id;
  uint joint_3_id;
  uint weight_0_id;
  uint weight_1_id;
  uint weight_2_id;
  uint weight_3_id;
  uint vertex_count;
  uint topology;
  uint reserved2[ 2 ];
};

#endif

