#ifndef GCT_LINEAR_ALLOCATOR_HPP
#define GCT_LINEAR_ALLOCATOR_HPP
#include <cstdint>
#include <unordered_map>
#include <boost/container/flat_set.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/linear_allocator_create_info.hpp>
#include <gct/property.hpp>

namespace gct {

class sized_linear_allocator :
  public property< linear_allocator_create_info > {
public:
  sized_linear_allocator(
    const linear_allocator_create_info &ci
  );
  [[nodiscard]] std::uint32_t allocate( std::uint32_t size );
  [[nodiscard]] std::uint32_t allocate( std::uint32_t size, std::uint32_t alignment );
  void release( std::uint32_t i );
  void reset();
  void to_json( nlohmann::json& ) const;
  [[nodiscard]] std::uint32_t get_tail() const { return tail; }
  [[nodiscard]] std::uint32_t size() const { return tail - deallocated.size(); }
  [[nodiscard]] std::uint32_t get_size( std::uint32_t ) const;
  void check();
private:
  boost::container::flat_set< std::uint32_t > deallocated;
  std::unordered_map< std::uint32_t, std::uint32_t > contiguous;
  std::uint32_t tail = 0u;
};
void to_json( nlohmann::json&, const sized_linear_allocator& );


class linear_allocator :
  public property< linear_allocator_create_info > {
public:
  linear_allocator(
    const linear_allocator_create_info &ci
  );
  [[nodiscard]] std::uint32_t allocate();
  void release( std::uint32_t i );
  void reset();
  void to_json( nlohmann::json& ) const;
  [[nodiscard]] std::uint32_t get_tail() const { return tail; }
  [[nodiscard]] std::uint32_t size() const { return tail - deallocated.size(); }
private:
  boost::container::flat_set< std::uint32_t > deallocated;
  std::uint32_t tail = 0u;
};
void to_json( nlohmann::json&, const linear_allocator& );

class reduced_linear_allocator :
  public property< linear_allocator_create_info > {
public:
  reduced_linear_allocator(
    const linear_allocator_create_info &ci
  );
  [[nodiscard]] std::uint32_t allocate( std::uint32_t count = 1u );
  void reset();
  void to_json( nlohmann::json& ) const;
  [[nodiscard]] std::uint32_t get_tail() const { return tail; }
  [[nodiscard]] std::uint32_t size() const { return tail; }
private:
  std::uint32_t tail = 0u;
};
void to_json( nlohmann::json&, const reduced_linear_allocator& );


}

#endif

