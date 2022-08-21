if(NOT ASTCENC_ROOT)
  find_program( ASTCENC astcenc-native )
else()
  find_program( ASTCENC astcenc-native NO_DEFAULT_PATH PATHS ${ASTCENC_ROOT}/ ${ASTCENC_ROOT}/bin/ )
endif()
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  ASTCENC
  REQUIRED_VARS ASTCENC
)

