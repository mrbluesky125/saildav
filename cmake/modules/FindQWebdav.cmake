# Find QWEBDAV - WEBDAV handling library for Qt
#
# This module defines
#  QWEBDAV_FOUND - whether the qsjon library was found
#  QWEBDAV_LIBRARIES - the qwebdav library
#  QWEBDAV_INCLUDE_DIR - the include path of the qwebdav library
#

if (QWEBDAV_INCLUDE_DIR AND QWEBDAV_LIBRARIES)

  # Already in cache
  set (QWEBDAV_FOUND TRUE)

else (QWEBDAV_INCLUDE_DIR AND QWEBDAV_LIBRARIES)

  if (NOT WIN32)
    # use pkg-config to get the values of QWEBDAV_INCLUDE_DIRS
    # and QWEBDAV_LIBRARY_DIRS to add as hints to the find commands.
    include (FindPkgConfig)
    pkg_check_modules (QWEBDAV QWebdav>=0.1)
  endif (NOT WIN32)

  find_library (QWEBDAV_LIBRARIES
    NAMES
    qwebdav
    PATHS
    ${QWEBDAV_LIBRARY_DIRS}
    ${LIB_INSTALL_DIR}
    ${KDE4_LIB_DIR}
  )

  find_path (QWEBDAV_INCLUDE_DIR
    NAMES
    qwebdav.h
    PATH_SUFFIXES
    qwebdav
    PATHS
    ${QWEBDAV_INCLUDE_DIRS}
    ${INCLUDE_INSTALL_DIR}
    ${KDE4_INCLUDE_DIR}
  )

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(QWEBDAV DEFAULT_MSG QWEBDAV_LIBRARIES QWEBDAV_INCLUDE_DIR)

endif (QWEBDAV_INCLUDE_DIR AND QWEBDAV_LIBRARIES)
