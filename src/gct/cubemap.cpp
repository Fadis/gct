#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <nlohmann/json.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/allocator.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/buffer.hpp>
#include <gct/gbuffer.hpp>
#include <gct/cubemap.hpp>

namespace gct {

std::pair< glm::mat4, std::array< glm::mat4, 6u > >
get_cubemap_matrix(
  const glm::vec3 &light_pos,
  float near,
  float far
) {
  auto proj = glm::perspective(
    1.5707963267948966f, 1.f, near, far
  );
  std::array< glm::mat4, 6u > view;
  view[ 0 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x + 1.f, light_pos.y, light_pos.z ),
    glm::vec3( 0.f, -1.f, 0.f )
  );
  view[ 1 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x - 1.f, light_pos.y, light_pos.z ),
    glm::vec3( 0.f, -1.f, 0.f )
  );
  view[ 2 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x , light_pos.y + 1.f, light_pos.z ),
    glm::vec3( 0.f, 0.f, 1.f )
  );
  view[ 3 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x, light_pos.y - 1.f, light_pos.z ),
    glm::vec3( 0.f, 0.f, -1.f )
  );
  view[ 4 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x , light_pos.y, light_pos.z + 1.f ),
    glm::vec3( 0.f, -1.f, 0.f )
  );
  view[ 5 ] = glm::lookAt(
    light_pos,
    glm::vec3( light_pos.x, light_pos.y, light_pos.z - 1.f ),
    glm::vec3( 0.f, -1.f, 0.f )
  );
  return std::make_pair( proj, view );
}

cubemap_images::cubemap_images(
  const std::shared_ptr< allocator_t > &allocator,
  const std::vector< image_create_info_t > &create_info,
  const std::shared_ptr< render_pass_t > &render_pass,
  const std::vector< vk::ClearValue > &clear_color
) {
  image_views.reserve( create_info.size() );
  for( const auto &ci: create_info ) {
    const auto aspect = format_to_aspect( ci.get_basic().format );
    auto basic = ci.get_basic();
    basic.flags |= vk::ImageCreateFlagBits::eCubeCompatible;
    basic.extent.height = basic.extent.width;
    basic.arrayLayers = 6u;
    images.push_back( allocator->create_image(
      gct::image_create_info_t()
        .set_basic( basic )
        .rebuild_chain(),
      VMA_MEMORY_USAGE_GPU_ONLY
    ) );
    image_views.push_back( std::array< std::shared_ptr< image_view_t >, 6u >{} );
    for( unsigned int i = 0u; i != 6u; ++i ) {
      image_views.back()[ i ] = images.back()->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( aspect )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( 1 )
                  .setBaseArrayLayer( i )
                  .setLayerCount( 1 )
              )
              .setViewType( to_image_view_type( images.back()->get_props().get_basic().imageType, images.back()->get_props().get_basic().arrayLayers, false ) )
          )
          .rebuild_chain()
      );
    }
    cube_image_views.push_back(
      images.back()->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( aspect )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( 1 )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( 6 )
              )
              .setViewType( vk::ImageViewType::eCube )
          )
          .rebuild_chain()
      )
    );
  }
  for( unsigned int i = 0u; i != 6u; ++i ) {
    gct::framebuffer_create_info_t fci;
    for( const auto &v: image_views ) {
      fci.add_attachment( v[ i ] );
    }
    framebuffers[ i ] = render_pass->get_framebuffer( std::move( fci ) );
    gct::render_pass_begin_info_t rpbi;
    for( unsigned int j = 0u; j != images.size(); ++j ) {
      rpbi.add_clear_value( clear_color[ j ] );
    }
    render_pass_begin_info[ i ] = rpbi;
    render_pass_begin_info[ i ]
      .set_render_pass( render_pass )
      .set_framebuffer( framebuffers[ i ] )
      .rebuild_chain();
  }
}
cubemap_matrix::cubemap_matrix(
  const glm::vec3 &center,
  float near_,
  float far_
) : near( near_ ), far( far_ ) {
  std::tie( proj, view ) = get_cubemap_matrix(
    center, near, far
  );
}
void cubemap_matrix::move_center( const glm::vec3 &center ) {
  std::tie( proj, view ) = get_cubemap_matrix(
    center, near, far
  );
}

void cubemap_matrix::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "projection" ] = nlohmann::json::array();
  for( unsigned int r = 0u; r != 4u; ++r ) {
    dest[ "projection" ].push_back( nlohmann::json::array() );
    for( unsigned int c = 0u; c != 4u; ++c ) {
      dest[ "projection" ][ r ].push_back( double( proj[ r ][ c ] ) );
    }
  }
  dest[ "view" ] = nlohmann::json::array();
  for( unsigned int v = 0u; v != 6u; ++v ) {
    dest[ "view" ].push_back( nlohmann::json::array() );
    for( unsigned int r = 0u; r != 4u; ++r ) {
      dest[ "view" ][ v ].push_back( nlohmann::json::array() );
      for( unsigned int c = 0u; c != 4u; ++c ) {
        dest[ "view" ][ v ][ r ].push_back( double( view[ v ][ r ][ c ] ) );
      }
    }
  }
}

cubemap_images2::cubemap_images2(
  const std::vector< std::shared_ptr< image_view_t > > &images
) {
  for( unsigned int swapchain_image_index = 0u; swapchain_image_index != images.size(); ++swapchain_image_index ) {
    const auto &ci = images[ swapchain_image_index ]->get_props().get_basic();
    if( ci.viewType == vk::ImageViewType::e2DArray && ci.subresourceRange.layerCount == 6u ) {
      cube_image_views.push_back(
        images[ swapchain_image_index ]->get_factory()->get_view(
          image_view_create_info_t()
            .set_basic(
              vk::ImageViewCreateInfo()
                .setSubresourceRange( ci.subresourceRange )
                .setViewType( vk::ImageViewType::eCube )
            )
            .rebuild_chain()
        )
      );
    }
    else if( ci.viewType == vk::ImageViewType::e2DArray && ci.subresourceRange.layerCount % 6u == 0u ) {
      cube_image_views.push_back(
        images[ swapchain_image_index ]->get_factory()->get_view(
          image_view_create_info_t()
            .set_basic(
              vk::ImageViewCreateInfo()
                .setSubresourceRange( ci.subresourceRange )
                .setViewType( vk::ImageViewType::eCubeArray )
            )
            .rebuild_chain()
        )
      );
    }
    else {
      throw -1;
    }
  }
}

void to_json( nlohmann::json &dest, const cubemap_matrix &src ) {
  src.to_json( dest );
}


}

