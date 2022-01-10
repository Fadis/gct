add_executable( vulkan-get_instance vulkan.cpp )
target_link_libraries( vulkan-get_instance ${Vulkan_LIBRARIES} gct )
add_executable( vulkan_hpp-get_instance vulkan_hpp.cpp )
target_link_libraries( vulkan_hpp-get_instance ${Vulkan_LIBRARIES} gct )
add_executable( gct-get_instance gct.cpp )
target_link_libraries( gct-get_instance ${Vulkan_LIBRARIES} gct )

