#ifndef GCT_COLOR_SPACE_HPP
#define GCT_COLOR_SPACE_HPP
/*
 * Copyright (C) 2024 Naomasa Matsubayashi
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

#include <memory>
#include <unordered_map>
#include <nlohmann/json_fwd.hpp>
#include <glm/mat4x4.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/setter.hpp>

namespace gct {
  
enum class color_space {
  unknown = 0,
  cie_xyz = 1,
  ntsc = 2,
  bt709 = 3, // aka srgb
  apple_rgb = 4,
  adobe_rgb = 5,
  display_p3 = 6,
  bt2020 = 7,
  aces_cg = 8
};

enum class color_gamma {
  unknown = 0,
  linear = 1,
  apple_rgb = 2, // 1.8
  srgb = 3, // 2.2
  bt709 = 4, // 2.4
  dci_p3 = 5, // 2.6
  hlg = 6, // ARIB STD-B67
  pq = 7 // SMPTE ST-2084
};

struct color_profile {
  LIBGCT_SETTER( space )
  LIBGCT_SETTER( gamma )
  LIBGCT_SETTER( max_intensity )
  color_space space = color_space::bt709;
  color_gamma gamma = color_gamma::srgb;
  float max_intensity = 100.0f;
};

using color_space_matrix_map = std::unordered_map< color_space, matrix_pool::matrix_descriptor >;

struct color_space_matrix {
  LIBGCT_SETTER( from )
  LIBGCT_SETTER( to )
  color_space_matrix_map from;
  color_space_matrix_map to;
};

[[nodiscard]] glm::mat4 get_rgb_to_xyz( color_space );
[[nodiscard]] glm::mat4 get_xyz_to_rgb( color_space );
[[nodiscard]] color_profile parse_color_space( const std::string_view &name );
[[nodiscard]] color_space_matrix allocate_color_space_matrix( matrix_pool &pool );
[[nodiscard]] float get_gamma_value( color_gamma );
[[nodiscard]] std::string to_string( color_space );
void to_json( nlohmann::json &to, color_space from );
void to_json( nlohmann::json &to, color_gamma from );
void to_json( nlohmann::json &to, const color_profile &from );
void to_json( nlohmann::json &to, const color_space_matrix &from );
};

#endif

