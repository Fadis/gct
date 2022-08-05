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
#include <gct/device.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/gltf.hpp>
namespace gct::gltf {
  document_t load_gltf(
    std::filesystem::path path,
    const std::shared_ptr< device_t > &device,
    command_buffer_recorder_t &command_buffer,
    const std::shared_ptr< allocator_t > &allocator,
    const std::shared_ptr< descriptor_pool_t > &descriptor_pool,
    const std::vector< std::shared_ptr< render_pass_t > > &render_pass,
    const std::filesystem::path &shader_dir,
    std::uint32_t subpass,
    uint32_t swapchain_size,
    int shader_mask,
    const std::vector< std::shared_ptr< buffer_t > > &dynamic_uniform_buffer,
    float aspect_ratio,
    bool ray_trace
  ) {
    fx::gltf::Document doc = fx::gltf::LoadFromText( path.string() );
    document_t document;
    shader_t shader;
    for( auto &path: std::filesystem::directory_iterator( shader_dir ) ) {
      auto flag = get_shader_flag( path.path() );
      if( flag ) {
        std::cout << path.path().string() << " をロード中..." << std::flush;
        shader.emplace(
          *flag,
          device->get_shader_module(
            path.path().string()
          )
        );
        std::cout << " OK" << std::endl;
      }
    }
    document.set_sampler( create_sampler(
      doc,
      device
    ) );
    document.set_default_sampler( create_default_sampler(
      device
    ) );
    document.set_image( create_image(
      doc,
      command_buffer,
      allocator,
      path.parent_path()
    ) );
    document.set_texture( create_texture(
      doc,
      document.image,
      document.sampler,
      document.default_sampler
    ) );
    document.set_mesh( create_mesh(
      doc, //
      device, //
      command_buffer, //
      allocator, //
      descriptor_pool, 
      render_pass,
      subpass,
      shader, //
      document.texture, //
      swapchain_size,
      shader_mask,
      dynamic_uniform_buffer
    ) );
    document.set_point_light( create_point_light(
      doc
    ) );
    document.set_camera( create_camera(
      doc,
      aspect_ratio
    ) );
    document.set_buffer( create_buffer(
      doc,
      command_buffer,
      allocator,
      path.parent_path(),
      ray_trace
    ) );
    document.set_node( create_node(
      doc,
      document.mesh
    ) );
    return document;
  }
}

