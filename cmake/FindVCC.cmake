if(NOT VCC_ROOT)
  find_program( VCC vcc )
else()
  find_program( VCC vcc NO_DEFAULT_PATH PATHS ${GLSLC_ROOT}/ ${GLSLC_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  VCC
  REQUIRED_VARS VCC
)

