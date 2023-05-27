/*
 * Copyright (C) 2022 Naomasa Matsubayashi
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
#include <iostream>
#include <vector>
#include <charconv>
#include <utility>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/font.hpp>
namespace gct {

  font command_buffer_recorder_t::load_font(
    std::filesystem::path path,
    const std::shared_ptr< allocator_t > &allocator
  ) {
    fx::gltf::Document doc = fx::gltf::LoadFromText( path.string() );
    if( doc.buffers.size() != 1u )
      throw invalid_font();
    auto buffer_path = std::filesystem::path( doc.buffers[ 0 ].uri );
    if( buffer_path.is_relative() ) buffer_path = path.parent_path() / buffer_path;
    std::cout << "フォント : " << buffer_path.string() << std::endl;
    auto buffer = load_buffer_from_file(
      allocator,
      buffer_path.string(),
      vk::BufferUsageFlagBits::eVertexBuffer|vk::BufferUsageFlagBits::eIndexBuffer
    );
    const auto vertex_buffer_view = std::find_if(
      doc.bufferViews.begin(),
      doc.bufferViews.end(),
      []( const auto &v ) {
        return v.target == fx::gltf::BufferView::TargetType::ArrayBuffer;
      }
    );
    if( vertex_buffer_view == doc.bufferViews.end() )
      throw invalid_font();
    const std::uint32_t vertex_buffer_offset = vertex_buffer_view->byteOffset;
    const std::uint32_t vertex_buffer_length = vertex_buffer_view->byteLength;
    if( vertex_buffer_length % ( sizeof( float ) * 3u ) )
      throw invalid_font();
    const auto vertex_buffer_accessor = std::find_if(
      doc.accessors.begin(),
      doc.accessors.end(),
      [i=std::size_t( std::distance( doc.bufferViews.begin(), vertex_buffer_view ) )]( const auto &v ) {
        return v.bufferView == i;
      }
    );
    if( vertex_buffer_accessor == doc.accessors.end() )
      throw invalid_font();
    if( vertex_buffer_accessor->componentType != fx::gltf::Accessor::ComponentType::Float )
      throw invalid_font();
    if( vertex_buffer_accessor->type != fx::gltf::Accessor::Type::Vec3 )
      throw invalid_font();
    std::uint32_t vertex_index_begin = buffer->get_props().get_basic().size;
    std::uint32_t vertex_index_end = 0u;
    for( auto &v: doc.bufferViews ) {
      if( v.target == fx::gltf::BufferView::TargetType::ElementArrayBuffer ) {
        vertex_index_begin = std::min( vertex_index_begin, v.byteOffset );
        vertex_index_end = std::max( vertex_index_end, v.byteOffset + v.byteLength );
      }
    }
    const std::uint32_t vertex_index_offset = vertex_index_begin;
    const std::uint32_t vertex_index_length = vertex_index_end - vertex_index_end;
    std::vector< std::pair< std::uint32_t, std::uint32_t > > vertex_index;
    glyph_index_hash hash;
    for( auto n: doc.nodes ) {
      std::uint32_t gid = 0u;
      auto conv_result = std::from_chars( n.name.data(), std::next( n.name.data(), n.name.size() ), gid );
      if( conv_result.ec != std::errc() ) {
        throw invalid_font();
      }
      if( n.mesh >= doc.meshes.size() )
        throw invalid_font();
      if( doc.meshes[ n.mesh ].primitives.size() != 1u )
        throw invalid_font();
      const auto &primitive = doc.meshes[ n.mesh ].primitives[ 0 ];
      if( primitive.indices >= doc.accessors.size() )
        throw invalid_font();
      const auto &accessor = doc.accessors[ primitive.indices ];
      if( accessor.componentType != fx::gltf::Accessor::ComponentType::UnsignedInt )
        throw invalid_font();
      if( accessor.type != fx::gltf::Accessor::Type::Scalar )
        throw invalid_font();
      if( accessor.bufferView >= doc.bufferViews.size() )
        throw invalid_font();
      const auto &buffer_view = doc.bufferViews[ accessor.bufferView ];
      if( buffer_view.buffer != 0u )
        throw invalid_font();
      if( buffer_view.target != fx::gltf::BufferView::TargetType::ElementArrayBuffer )
        throw invalid_font();
      const auto offset = buffer_view.byteOffset - vertex_index_offset;
      constexpr static std::uint32_t face_size = ( sizeof( std::uint32_t ) * 3u );
      if( offset % face_size )
        throw invalid_font();
      if( buffer_view.byteLength % face_size )
        throw invalid_font();
      hash.insert(
        std::make_pair(
          gid,
          glyph_index{
            std::uint32_t( offset / sizeof( std::uint32_t ) ),
            std::uint32_t( buffer_view.byteLength / sizeof( std::uint32_t ) )
          }
        )
      );
    }
    return font(
      std::move( buffer ),
      std::move( hash ),
      vertex_index_begin
    );
  }
}

