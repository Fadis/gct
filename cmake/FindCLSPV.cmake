if(NOT CLSPV_ROOT)
  find_program( CLSPV clspv )
else()
  find_program( CLSPV clspv NO_DEFAULT_PATH PATHS ${GLSLC_ROOT}/ ${GLSLC_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  CLSPV
  REQUIRED_VARS CLSPV
)

