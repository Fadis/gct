add_executable( list_displays
  list_displays.cpp
)

target_link_libraries(
  list_displays
  gct
  Threads::Threads
  ${pkgcfg_lib_Vulkan_vulkan}
)

add_executable( select_display
  select_display.cpp
)

target_link_libraries(
  select_display
  gct
  Threads::Threads
  ${pkgcfg_lib_Vulkan_vulkan}
)

