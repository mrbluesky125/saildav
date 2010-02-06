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

# Use it
INCLUDE( ${QT_USE_FILE} )

# Include the library include directories, and the current build directory (moc)
INCLUDE_DIRECTORIES(
	../../src
	${CMAKE_CURRENT_BINARY_DIR}
)

SET(explorer_SRCS main.cpp mainwindow.cpp propertiesdialog.cpp)
SET(explorer_UIS mainwindow.ui propertiesdialog.ui)
SET(explorer_MOC_HDRS mainwindow.h)
SET(explorer_RCS explorer.qrc)

QT4_WRAP_CPP(MOC_SOURCE ${explorer_MOC_HDRS})
QT4_WRAP_UI(UI_HDRS ${explorer_UIS})
QT4_ADD_RESOURCES(RC_SRCS ${explorer_RCS} )

ADD_EXECUTABLE(
	explorer
	${explorer_SRCS}
	${UI_HDRS}
	${MOC_SOURCE}
	${RC_SRCS}
)

TARGET_LINK_LIBRARIES(
	explorer
	${QT_LIBRARIES}
	qwebdav
)

