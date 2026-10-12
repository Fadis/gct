#ifndef GCT_TERRAIN_MESH_H
#define GCT_TERRAIN_MESH_H
/*
 * Copyright (C) 2026 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gct/setter.hpp>
#include <nlohmann/json_fwd.hpp>

namespace gct {

struct terrain_vertex {
  LIBGCT_SETTER( position )
  LIBGCT_SETTER( area )
  LIBGCT_SETTER( border )
  glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f };
  float area = 0.0f;
  bool border = false;
};

void to_json( nlohmann::json&, const terrain_vertex& );
void from_json( const nlohmann::json&, terrain_vertex& );

struct terrain_mesh {
  terrain_mesh() {}
  terrain_mesh( std::size_t point_count, float area );
  void dump() const;
  std::vector< terrain_vertex > vertex;
  std::vector< glm::ivec2 > edge;
  std::vector< glm::ivec3 > triangle;
};

void to_json( nlohmann::json&, const terrain_mesh& );
void from_json( const nlohmann::json&, terrain_mesh& );

}

#endif

