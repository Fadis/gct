#ifndef GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_TYPE_H

struct primitive_resource_index_type {
  vec4 base_color; // アルベド
  vec4 emissive; // エミッシブ
  float metallic; // メタリック
  float roughness; // ラフネス
  float normal_scale; // 法線マップのスケール
  float occlusion_strength; // 遮蔽度
  uint base_color_texture; // アルベドのテクスチャ
  uint metallic_roughness_texture; // マテリアルのテクスチャ
  uint normal_texture; // 法線のテクスチャ
  uint occlusion_texture; // 遮蔽度のテクスチャ
  uint emissive_texture; // エミッシブのテクスチャ
  uint mesh; // メッシュシェーダーで必要になる情報
  uint cull;
  uint reserved;
};

#endif

