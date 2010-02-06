# This file is part of the KDE project
# Copyright (C) 2009-2010 Corentin Chary <corentin.chary@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

include_directories (./)

install(FILES
  webdav.h
  webdav_url_info.h
  webdav_export.h
  QWebdav
  QWebdavUrlInfos
  DESTINATION ${INCLUDE_INSTALL_DIR}/qwebdav COMPONENT Devel
)

set(qwebdav_MOC_HDRS
  webdav.h
)

qt4_wrap_cpp(qwebdav_MOC_SRCS ${qwebdav_MOC_HDRS})

set (qwebdav_SRCS webdav.cpp webdav_url_info.cpp)

add_library (qwebdav SHARED ${qwebdav_SRCS} ${qwebdav_MOC_SRCS})
target_link_libraries( qwebdav ${QT_LIBRARIES})

set_target_properties(qwebdav PROPERTIES
                      VERSION ${QWEBDAV_LIB_MAJOR_VERSION}.${QWEBDAV_LIB_MINOR_VERSION}.${QWEBDAV_LIB_PATCH_VERSION}
                      SOVERSION ${QWEBDAV_LIB_MAJOR_VERSION}
                      DEFINE_SYMBOL QWEBDAV_MAKEDLL
                      )

INSTALL(TARGETS qwebdav
   LIBRARY DESTINATION ${LIB_INSTALL_DIR}
   RUNTIME DESTINATION bin
   ARCHIVE DESTINATION  ${LIB_INSTALL_DIR}
)
