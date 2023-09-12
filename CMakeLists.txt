cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
PROJECT(
  libgct
  LANGUAGES CXX C
  HOMEPAGE_URL https://github.com/Fadis/gct
  DESCRIPTION "GPU Computing Toolkit"
  VERSION 0.1.0
)
cmake_policy( SET CMP0074 NEW )
include(CheckIncludeFiles)
include(FindPkgConfig)
include(CheckCXXSourceCompiles)
include(GNUInstallDirs)

# build type
#set(CMAKE_BUILD_TYPE Debug)

set( ADDITIONAL_COMPILE_FLAGS )
option( ENABLE_ADDRSAN "build with address sanitizer" OFF )
if( ENABLE_ADDRSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=address" )
  if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    list(APPEND ADDITIONAL_COMPILE_FLAGS "-static-libasan" )
  endif()
endif()
option( ENABLE_PCOMPSAN "build with pointer compare sanitizer" OFF )
if( ENABLE_PCOMPSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=pointer-compare" )
endif()
option( ENABLE_PSUBSAN "build with pointer subtract sanitizer" OFF )
if( ENABLE_PSUBSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=pointer-subtract" )
endif()
option( ENABLE_THREADSAN "build with thread sanitizer" OFF )
if( ENABLE_THREADSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=thread" )
endif()
option( ENABLE_LEAKSAN "build with leak sanitizer" OFF )
if( ENABLE_LEAKSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=leak" )
endif()
option( ENABLE_UBSAN "build with undefined behavior sanitizer" OFF )
if( ENABLE_UBSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined" )
endif()
option( ENABLE_WERROR "build with -Werror" ON )
list( APPEND ADDITIONAL_COMPILE_FLAGS "-Wall" )
if( ENABLE_WERROR )
  list( APPEND ADDITIONAL_COMPILE_FLAGS "-Werror" )
endif()
set( CMAKE_CXX_STANDARD "20" )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
option( ENABLE_EXAMPLES "build examples" OFF )

# compile option (Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ${ADDITIONAL_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g ${ADDITIONAL_COMPILER_FLAGS}")

# compile option (Release)
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O2 ${ADDITIONAL_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "-march=native -O2 ${ADDITIONAL_COMPILER_FLAGS}")

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(Threads REQUIRED)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.68.0 COMPONENTS program_options context system REQUIRED )
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(Vulkan REQUIRED vulkan)
find_package(JSON REQUIRED)
find_package(FXGLTF REQUIRED)
find_package(OpenImageIO REQUIRED)
pkg_check_modules(VULKAN2JSON REQUIRED vulkan2json)
find_package(GLSLC REQUIRED)
find_package(ASTCEnc REQUIRED)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)
pkg_check_modules(ICUUC REQUIRED icu-uc)
pkg_check_modules(LIBINPUT REQUIRED libinput)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
pkg_check_modules(SDBUSCPP REQUIRED sdbus-c++)
if( LIBINPUT_VERSION VERSION_LESS "1.19" )
  add_definitions( -DGCT_LIBINPUT_NO_POINTER_WHEEL )
  add_definitions( -DGCT_LIBINPUT_NO_GESTURE_HOLD )
endif()
find_package(NNEF REQUIRED)


configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/external/VulkanMemoryAllocator/include/vk_mem_alloc.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/vk_mem_alloc.h
  COPYONLY
)
INSTALL(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/vk_mem_alloc.h
  DESTINATION
  include/gct/
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/external/SPIRV-Reflect/spirv_reflect.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/spirv_reflect.h
  COPYONLY
)
INSTALL(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/spirv_reflect.h
  DESTINATION
  include/gct/
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/external/SPIRV-Reflect/include/spirv/unified1/spirv.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/include/spirv/unified1/spirv.h
  COPYONLY
)
INSTALL(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/include/spirv/unified1/spirv.h
  DESTINATION
  include/gct/include/spirv/unified1/
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/external/half/include/half.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/half.hpp
  COPYONLY
)
INSTALL(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/gct/half.hpp
  DESTINATION
  include/gct/
)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/minimp4
  ${CMAKE_CURRENT_SOURCE_DIR}/external/ttf2mesh
  ${CMAKE_CURRENT_SOURCE_DIR}/external/tqdm-cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/external/hdmap/include
  ${Boost_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
  ${JSON_INCLUDE_DIRS}
  ${FXGLTF_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS}
  ${OIIO_INCLUDE_DIR}
  ${VULKAN2JSON_INCLUDE_DIR}
  ${HARFBUZZ_INCLUDE_DIRS}
  ${ICUUC_INCLUDE_DIRS}
  ${X11_xkbcommon_INCLUDE_PATH}
  ${NNEF_INCLUDE_DIRS}
)
link_directories(
  ${Boost_LIBRARY_DIRS}
  ${Vulkan_LIBRARY_DIRS}
  ${OIIO_LIBRARY_DIR}
  ${VULKAN2JSON_LIBRARY_DIR}
  ${HARFBUZZ_LIBRARY_DIR}
  ${ICUUC_LIBRARY_DIR}
)

add_definitions( -DVK_ENABLE_BETA_EXTENSIONS -DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 )

configure_file(libgct.pc.in libgct.pc @ONLY)
install(
  FILES
  ${CMAKE_BINARY_DIR}/libgct.pc
  DESTINATION
  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
)
configure_file(libgct_glfw.pc.in libgct_glfw.pc @ONLY)
install(
  FILES
  ${CMAKE_BINARY_DIR}/libgct_glfw.pc
  DESTINATION
  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
)

if( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  execute_process(
    COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
endif()

set( EASYPACK_VENDOR "fadis" )
set( EASYPACK_RELEASE "1" )
set( EASYPACK_MAINTAINER "Naomasa Matsubayashi <foo@exmple.com>" )
set( EASYPACK_COMPRESSION_TYPE "xz" )
set( EASYPACK_LICENSE_NAME "MIT" )
include(EasyPack)

include(AddShader)
include(AddASTC)

subdirs( include src )

