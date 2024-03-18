#ifndef GCT_LINEAR_ALLOCATOR_HPP
#define GCT_LINEAR_ALLOCATOR_HPP
#include <cstdint>
#include <boost/container/flat_set.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/linear_allocator_create_info.hpp>

namespace gct {

class linear_allocator {
public:
  linear_allocator(
    const linear_allocator_create_info &ci
  );
  const linear_allocator_create_info &get_props() const {
    return props;
  }
  std::uint32_t allocate();
  void release( std::uint32_t i );
  void reset();
  void to_json( nlohmann::json& ) const;
  std::uint32_t get_tail() const { return tail; }
  std::uint32_t size() const { return tail - deallocated.size(); }
private:
  linear_allocator_create_info props;
  boost::container::flat_set< std::uint32_t > deallocated;
  std::uint32_t tail = 0u;
};
void to_json( nlohmann::json&, const linear_allocator& );

class reduced_linear_allocator {
public:
  reduced_linear_allocator(
    const linear_allocator_create_info &ci
  );
  const linear_allocator_create_info &get_props() const {
    return props;
  }
  std::uint32_t allocate();
  void reset();
  void to_json( nlohmann::json& ) const;
  std::uint32_t get_tail() const { return tail; }
  std::uint32_t size() const { return tail; }
private:
  linear_allocator_create_info props;
  std::uint32_t tail = 0u;
};
void to_json( nlohmann::json&, const reduced_linear_allocator& );


}

#endif

