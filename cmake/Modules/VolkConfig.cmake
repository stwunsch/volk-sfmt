INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_VOLK volk_sfmt)

FIND_PATH(
    VOLK_INCLUDE_DIRS
    NAMES volk_sfmt/volk_sfmt.h
    HINTS $ENV{VOLK_DIR}/include
        ${PC_VOLK_INCLUDEDIR}
    PATHS /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    VOLK_LIBRARIES
    NAMES volk_sfmt
    HINTS $ENV{VOLK_DIR}/lib
        ${PC_VOLK_LIBDIR}
    PATHS /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VOLK DEFAULT_MSG VOLK_LIBRARIES VOLK_INCLUDE_DIRS)
MARK_AS_ADVANCED(VOLK_LIBRARIES VOLK_INCLUDE_DIRS)
