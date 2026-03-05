if(NOT TODDS_ROOT)
  find_program( TODDS todds )
else()
  find_program( TODDS todds NO_DEFAULT_PATH PATHS ${TODDS_ROOT}/ ${TODDS_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  TODDS
  REQUIRED_VARS TODDS
)

