if(NOT DXC_ROOT)
  find_program( DXC dxc )
else()
  find_program( DXC dxc NO_DEFAULT_PATH PATHS ${GLSLC_ROOT}/ ${GLSLC_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  DXC
  REQUIRED_VARS DXC
)

