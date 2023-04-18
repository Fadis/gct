#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/image_view.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/render_pass.hpp>
#include <gct/framebuffer.hpp>
#include <gct/allocator.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/voxel_image.hpp>

namespace gct {
voxel_image::voxel_image(
  const std::shared_ptr< allocator_t > &allocator,
  unsigned int size,
  vk::Format format
) {
  image = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e3D )
          .setFormat( format )
          .setExtent( { 1u << size, 1u << size, 1u << size } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  )->get_view( vk::ImageAspectFlagBits::eColor );

  render_pass = get_device( *allocator ).get_render_pass(
    gct::render_pass_create_info_t()
      .add_subpass(
        gct::subpass_description_t()
      )
      .rebuild_chain()
  );
  framebuffer = render_pass->get_framebuffer(
    gct::framebuffer_create_info_t()
      .set_basic(
        vk::FramebufferCreateInfo()
          .setWidth( 1 << size )
          .setHeight( 1 << size )
          .setLayers( 1 )
      )
  );
  rpbi = gct::render_pass_begin_info_t()
    .set_render_pass( render_pass )
    .set_framebuffer( framebuffer );
  viewport =
    vk::Viewport()
      .setWidth( 1 << size )
      .setHeight( 1 << size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  scissor =
    vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D( 1 << size, 1 << size ) );
}

voxel_matrix::voxel_matrix(
  glm::vec3 min,
  glm::vec3 max
) {
  input_projection[ 2 ] =
    glm::translate(
      glm::scale(
        glm::translate(
          glm::mat4( 1.0 ),
          glm::vec3( 0.0, 0.0, 0.5 )
        ),
        glm::vec3(
          2.f/( max.x - min.x ),
          2.f/( max.y - min.y ),
          1.f/( max.z - min.z )
        )
      ),
      glm::vec3(
        -( max.x + min.x )/2,
        -( max.y + min.y )/2, 
        -( max.z + min.z )/2
      )
    ) * glm::mat4(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    );
  input_projection[ 1 ] =
    glm::translate(
      glm::scale(
        glm::translate(
          glm::mat4( 1.0 ),
          glm::vec3( 0.0, 0.0, 0.5 )
        ),
        glm::vec3(
          2.f/( max.x - min.x ),
          2.f/( max.z - min.z ),
          1.f/( max.y - min.y )
        )
      ),
      glm::vec3(
        -( max.x + min.x )/2,
        -( max.z + min.z )/2, 
        -( max.y + min.y )/2
      ) 
    ) * glm::mat4(
      1, 0, 0, 0,
      0, 0, 1, 0,
      0, 1, 0, 0,
      0, 0, 0, 1
    );
  input_projection[ 0 ] =
    glm::translate(
      glm::scale(
        glm::translate(
          glm::mat4( 1.0 ),
          glm::vec3( 0.0, 0.0, 0.5 )
        ),
        glm::vec3(
          2.f/( max.z - min.z ),
          2.f/( max.y - min.y ),
          1.f/( max.x - min.x )
        )
      ),
      glm::vec3(
        -( max.z + min.z )/2,
        -( max.y + min.y )/2, 
        -( max.x + min.x )/2
      ) 
    ) * glm::mat4(
      0, 0, 1, 0,
      0, 1, 0, 0,
      1, 0, 0, 0,
      0, 0, 0, 1
    );
  output_projection =
  glm::translate(
    glm::scale(
      glm::mat4( 1.0 ),
      glm::vec3(
        2.f/( max.x - min.x ),
        2.f/( max.y - min.y ),
        2.f/( max.z - min.z )
      )
    ),
    glm::vec3(
      -( max.x + min.x )/2,
      -( max.y + min.y )/2, 
      -( max.z + min.z )/2
    ) 
  );
}

}

