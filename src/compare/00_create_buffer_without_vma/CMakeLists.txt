add_executable( vulkan-create_buffer vulkan.cpp )
target_link_libraries( vulkan-create_buffer ${Vulkan_LIBRARIES} gct )
add_executable( vulkan_hpp-create_buffer vulkan_hpp.cpp )
target_link_libraries( vulkan_hpp-create_buffer ${Vulkan_LIBRARIES} gct )

