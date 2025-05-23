#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_TYPE_H

#include <gct/type_id.h>

struct accessor_type {
  uint enabled; // この頂点の情報が存在するかどうか
  uint vertex_buffer; // どのストレージバッファから読むか
  uint type; // 書かれている情報の型
  uint normalized; // 書かれている情報が整数の場合に0.0から1.0の範囲に正規化すべきか
  uint component_count; // 書かれている情報のべクトルの要素数
  uint offset; // ストレージバッファの先頭から何要素目から読むべきか
  uint stride; // 1要素読む度に何要素分進むべきか
  uint reserved;
};

#endif

