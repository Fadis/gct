if(NOT BETSY_ROOT)
  find_program( BETSY betsy )
else()
  find_program( BETSY betsy NO_DEFAULT_PATH PATHS ${BETSY_ROOT}/ ${BETSY_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  BETSY
  REQUIRED_VARS BETSY
)

