/*
 * Copyright (C) 2020 Naomasa Matsubayashi
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
#include <vulkan/vulkan.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#include <fx/gltf.h>
#pragma GCC diagnostic pop
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/exception.hpp>
#include <gct/buffer.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/render_pass.hpp>
#include <gct/gltf.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/device.hpp>
#include <glm/gtx/string_cast.hpp>
namespace gct::gltf {

  glm::tmat4x4< float > to_matrix(
    const std::array< float, 3 > &t,
    const std::array< float, 4 > &r,
    const std::array< float, 3 > &s
  ) {
    glm::tmat4x4< float > dest( 1.0f );
    dest = glm::translate( dest, glm::tvec3< float >( t[ 0 ], t[ 1 ], t[ 2 ] ) );
    auto rot = glm::tquat< float >( r[ 3 ], r[ 0 ], r[ 1 ], r[ 2 ] );
    glm::tmat4x4< float > quatMat = glm::mat4_cast( rot );
    dest *= quatMat;
    return glm::scale( dest, glm::tvec3< float >( s[ 0 ], s[ 1 ], s[ 2 ] ) );
  }
  glm::tmat4x4< float > to_matrix(
    const std::array< float, 16 > &t
  ) {
    return glm::tmat4x4< float >(
      t[ 0 ], t[ 1 ], t[ 2 ], t[ 3 ],
      t[ 4 ], t[ 5 ], t[ 6 ], t[ 7 ],
      t[ 8 ], t[ 9 ], t[ 10 ], t[ 11 ],
      t[ 12 ], t[ 13 ], t[ 14 ], t[ 15 ]
    );
  }


  node_t create_node(
    const fx::gltf::Document &doc,
    int32_t index,
    const glm::mat4 &upper,
    const meshes_t &meshes
  ) {
    if( index < 0 || doc.nodes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたnodeが存在しない", __FILE__, __LINE__ );
    const auto &node = doc.nodes[ index ];
    node_t node_;
    node_.set_gltf( node );
    if( node.matrix != fx::gltf::defaults::IdentityMatrix ) {
      node_.set_matrix( upper * to_matrix( node.matrix ) );
    }
    else {
      node_.set_matrix( upper * to_matrix( node.translation, node.rotation, node.scale ) );
    }
    glm::vec3 min(
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max()
    );
    glm::vec3 max(
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min()
    );
    if( node.mesh >= 0 ) {
      if( doc.meshes.size() <= size_t( node.mesh ) ) throw invalid_gltf( "参照されたmeshが存在しない", __FILE__, __LINE__ );
      node_.set_mesh( node.mesh );
      node_.set_has_mesh( true );
      auto &mesh = meshes[ node.mesh ];
      auto a = glm::vec3( node_.matrix * glm::vec4( mesh.min[ 0 ], mesh.min[ 1 ], mesh.min[ 2 ], 1.0f ) );
      auto b = glm::vec3( node_.matrix * glm::vec4( mesh.min[ 0 ], mesh.min[ 1 ], mesh.max[ 2 ], 1.0f ) );
      auto c = glm::vec3( node_.matrix * glm::vec4( mesh.min[ 0 ], mesh.max[ 1 ], mesh.min[ 2 ], 1.0f ) );
      auto d = glm::vec3( node_.matrix * glm::vec4( mesh.min[ 0 ], mesh.max[ 1 ], mesh.max[ 2 ], 1.0f ) );
      auto e = glm::vec3( node_.matrix * glm::vec4( mesh.max[ 0 ], mesh.min[ 1 ], mesh.min[ 2 ], 1.0f ) );
      auto f = glm::vec3( node_.matrix * glm::vec4( mesh.max[ 0 ], mesh.min[ 1 ], mesh.max[ 2 ], 1.0f ) );
      auto g = glm::vec3( node_.matrix * glm::vec4( mesh.max[ 0 ], mesh.max[ 1 ], mesh.min[ 2 ], 1.0f ) );
      auto h = glm::vec3( node_.matrix * glm::vec4( mesh.max[ 0 ], mesh.max[ 1 ], mesh.max[ 2 ], 1.0f ) );
      min = glm::vec3(
        std::min( { a[ 0 ], b[ 0 ], c[ 0 ], d[ 0 ], e[ 0 ], f[ 0 ], g[ 0 ], h[ 0 ] } ),
        std::min( { a[ 1 ], b[ 1 ], c[ 1 ], d[ 1 ], e[ 1 ], f[ 1 ], g[ 1 ], h[ 1 ] } ),
        std::min( { a[ 2 ], b[ 2 ], c[ 2 ], d[ 2 ], e[ 2 ], f[ 2 ], g[ 2 ], h[ 2 ] } )
      );
      max = glm::vec3(
        std::max( { a[ 0 ], b[ 0 ], c[ 0 ], d[ 0 ], e[ 0 ], f[ 0 ], g[ 0 ], h[ 0 ] } ),
        std::max( { a[ 1 ], b[ 1 ], c[ 1 ], d[ 1 ], e[ 1 ], f[ 1 ], g[ 1 ], h[ 1 ] } ),
        std::max( { a[ 2 ], b[ 2 ], c[ 2 ], d[ 2 ], e[ 2 ], f[ 2 ], g[ 2 ], h[ 2 ] } )
      );
    }
    else
      node_.set_has_mesh( false );
    if( !node.extensionsAndExtras.is_null() ) {
      if( node.extensionsAndExtras.find( "extensions" ) != node.extensionsAndExtras.end() ) {
        auto &ext = node.extensionsAndExtras[ "extensions" ];
        if( ext.find( "KHR_lights_punctual" ) != ext.end() ) {
          auto &light = ext[ "KHR_lights_punctual" ];
          if( light.find( "light" ) != light.end() ) {
            node_.set_light( int( light[ "light" ] ) );
            node_.set_has_light( true );
          }
          else
            node_.set_has_light( false );
        }
        else
          node_.set_has_light( false );
      }
      else
        node_.set_has_light( false );
    }
    else
      node_.set_has_light( false );
    if( node.camera != -1 ) {
      node_.set_camera( node.camera );
      node_.set_has_camera( true );
    }
    else node_.set_has_camera( false );
    for( const auto c: node.children ) {
      node_.children.push_back(
        create_node( doc, c, node_.matrix, meshes )
      );
      min[ 0 ] = std::min( min[ 0 ], node_.children.back().min[ 0 ] );
      min[ 1 ] = std::min( min[ 1 ], node_.children.back().min[ 1 ] );
      min[ 2 ] = std::min( min[ 2 ], node_.children.back().min[ 2 ] );
      max[ 0 ] = std::max( max[ 0 ], node_.children.back().max[ 0 ] );
      max[ 1 ] = std::max( max[ 1 ], node_.children.back().max[ 1 ] );
      max[ 2 ] = std::max( max[ 2 ], node_.children.back().max[ 2 ] );
    }
    node_.set_min( min );
    node_.set_max( max );
    return node_;
  }
  node_t create_node(
    const fx::gltf::Document &doc,
    const meshes_t &mesh
  ) {
    int32_t index = doc.scene;
    if( index < 0 || doc.scenes.size() <= size_t( index ) ) throw invalid_gltf( "参照されたsceneが存在しない", __FILE__, __LINE__ );
    const auto &scene = doc.scenes[ index ];
    node_t root;
    root.set_matrix( glm::mat4(1,0,0,0,0,1,0,0,0,0,-1,0,0,0,0,1) );
    glm::vec3 min(
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max(),
      std::numeric_limits< float >::max()
    );
    glm::vec3 max(
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min(),
      std::numeric_limits< float >::min()
    );
    for( const auto c: scene.nodes ) {
      root.children.push_back(
        create_node( doc, c, root.matrix, mesh )
      );
      min[ 0 ] = std::min( min[ 0 ], root.children.back().min[ 0 ] );
      min[ 1 ] = std::min( min[ 1 ], root.children.back().min[ 1 ] );
      min[ 2 ] = std::min( min[ 2 ], root.children.back().min[ 2 ] );
      max[ 0 ] = std::max( max[ 0 ], root.children.back().max[ 0 ] );
      max[ 1 ] = std::max( max[ 1 ], root.children.back().max[ 1 ] );
      max[ 2 ] = std::max( max[ 2 ], root.children.back().max[ 2 ] );
    }
    root.set_min( min );
    root.set_max( max );
    return root;
  }
  void draw_node(
    const command_buffer_recorder_t &rec,
    const node_t &node,
    const meshes_t &meshes,
    const buffers_t &buffers,
    uint32_t current_frame,
    uint32_t pipeline_index
  ) {
    for( const auto &n: node.children )
      draw_node( rec, n, meshes, buffers, current_frame, pipeline_index );
    if( node.has_mesh ) {
      const auto &mesh = meshes[ node.mesh ];
      for( const auto &primitive: mesh.primitive ) {
        rec->bindPipeline( vk::PipelineBindPoint::eGraphics, **primitive.pipeline[ pipeline_index ] );
        auto pc = push_constants_t()
          .set_world_matrix( node.matrix )
          .set_fid( pipeline_index );
        rec->pushConstants(
          **primitive.pipeline_layout,
          vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment,
          0,
          sizeof( push_constants_t ),
          &pc
        );
        std::vector< vk::DescriptorSet > descriptor_set{
          **primitive.descriptor_set[ current_frame ]
        };
        rec->bindDescriptorSets(
          vk::PipelineBindPoint::eGraphics,
          **primitive.pipeline_layout,
          0,
          descriptor_set,
          {}
        );
        for( const auto &[bind_point,view]: primitive.vertex_buffer ) {
          std::vector< vk::Buffer > vb{ **buffers[ view.index ] };
          rec->bindVertexBuffers( bind_point, vb, view.offset );
        }
        if( !primitive.indexed ) {
          rec->draw( primitive.count, 1, 0, 0 );
        }
        else {
          rec->bindIndexBuffer( **buffers[ primitive.index_buffer.index ], primitive.index_buffer.offset, primitive.index_buffer_type );
          rec->drawIndexed( primitive.count, 1, 0, 0, 0 );
        }
      }
    }
  }
  point_lights_t get_point_lights(
    const node_t &node,
    const point_lights_t &lights
  ) {
    point_lights_t lights_;
    for( const auto &n: node.children ) {
      auto child_lights = get_point_lights( n, lights );
      lights_.insert( lights_.end(), child_lights.begin(), child_lights.end() );
    }
    if( node.has_light ) {
      if( size_t( node.light ) < lights.size() ) {
        auto location = node.matrix * glm::vec4( 0.f, 0.f, 0.f, 1.f );
        auto light = lights[ node.light ];
        lights_.emplace_back(
          light.set_location( glm::vec3( location[ 0 ], location[ 1 ], location[ 2 ] ) )
        );
      }
    }
    return lights_;
  }
  cameras_t get_cameras(
    const node_t &node,
    const cameras_t &cameras
  ) {
    cameras_t cameras_;
    for( const auto &n: node.children ) {
      auto child_cameras = get_cameras( n, cameras );
      cameras_.insert( cameras_.end(), child_cameras.begin(), child_cameras.end() );
    }
    if( node.has_camera ) {
      if( size_t( node.camera ) < cameras.size() ) {
        auto camera_pos = node.matrix * glm::vec4( 0.f, 0.f, 0.f, 1.f );
        auto camera_dir = node.matrix * glm::vec4( 0.f, 0.f, -1.f, 1.f );
        cameras_.emplace_back(
          camera_t()
            .set_camera_pos( glm::vec3( camera_pos[ 0 ], camera_pos[ 1 ], camera_pos[ 2 ] ) )
            .set_camera_direction( glm::vec3( camera_dir[ 0 ], camera_dir[ 1 ], camera_dir[ 2 ] ) )
        );
      }
    }
    return cameras_;
  }
}

