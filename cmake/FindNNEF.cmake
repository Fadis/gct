if( NOT NNEF_ROOT )
  find_path (NNEF_INCLUDE_DIRS NAMES nnef.h PATHS /usr/include /usr/local/include )
  find_library (NNEF_LIBRARY NAMES nnef )
else()
  find_path (NNEF_INCLUDE_DIRS NAMES nnef.h PATHS ${NNEF_ROOT}/include )
  find_library (NNEF_LIBRARY NAMES nnef PATHS ${NNEF_ROOT}/lib ${NNEF_ROOT}/lib64 ${NNEF_ROOT} NO_DEFAULT_PATH )
endif()
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  NNEF DEFAULT_MSG
  NNEF_INCLUDE_DIRS NNEF_LIBRARY
)
set(NNEF_LIBRARIES "${NNEF_LIBRARY}")
mark_as_advanced(NNEF_INCLUDE_DIR NNEF_LIBRARIES NNEF_LIBRARY)

