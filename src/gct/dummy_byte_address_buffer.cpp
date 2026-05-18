#include <gct/half.hpp>
#include <gct/dummy_byte_address_buffer.hpp>

namespace gct {

std::unordered_map< std::uint32_t, std::vector< std::uint8_t > > &
get_vertex_buffer() {
  static std::unordered_map< std::uint32_t, std::vector< std::uint8_t > > instance;
  return instance;
}

std::uint16_t vertex_buffer_load_u16( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< std::uint16_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 1 ) + 0 ];
}

std::int16_t vertex_buffer_load_i16( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< std::int16_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 1 ) + 0 ];
}

std::uint8_t vertex_buffer_load_u8( std::uint8_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< std::uint8_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset ) + 0 ];
}

std::int8_t vertex_buffer_load_i8( std::uint8_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< std::int8_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset ) + 0 ];
}

float vertex_buffer_load_f32( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< float* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 0 ];
}

float vertex_buffer_load_f16( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< half_float::half* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 1 ) + 0 ];
}


std::uint32_t vertex_buffer_load( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 0 ];
}

glm::uvec2 vertex_buffer_load2( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return glm::uvec2(
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 0 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 1 ]
  );
}

glm::uvec3 vertex_buffer_load3( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return glm::uvec3(
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 0 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 1 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 2 ]
  );
}

glm::uvec4 vertex_buffer_load4( std::uint32_t vertex_buffer, std::uint32_t offset ) {
  return glm::uvec4(
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 0 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 1 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 2 ],
    reinterpret_cast< std::uint32_t* >( get_vertex_buffer()[ vertex_buffer ].data() )[ ( offset >> 2 ) + 3 ]
  );
}

}

