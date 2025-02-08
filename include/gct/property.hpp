#ifndef GCT_PROPERTY_HPP
#define GCT_PROPERTY_HPP

namespace gct {
  template< typename P >
  class property {
  public:
    using property_type = property< P >;
    using raw_property_type = P;
    property(
      const raw_property_type &create_info
    ) : props( create_info ) {}
    [[nodiscard]] const raw_property_type &get_props() const { return props; }
  protected:
// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
    raw_property_type props;
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)
  };
}

#endif

