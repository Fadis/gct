if(NOT GLSLC_ROOT)
  find_program( GLSLC glslc )
else()
  find_program( GLSLC glslc NO_DEFAULT_PATH PATHS ${GLSLC_ROOT}/ ${GLSLC_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  GLSLC
  REQUIRED_VARS GLSLC
)

