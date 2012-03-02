TEMPLATE = lib
TARGET = qmlqwebdavplugin

CONFIG += qt plugin

QT += xml network declarative

uri = Webdav

include(abstracttreemodel/abstracttreemodel.pri)
include(qxmlstreambase/qxmlstreambase.pri)

HEADERS += webdav.h \
    webdavfileinfo.h \
    webdavmodel.h \
    webdavplugin.h

SOURCES += webdav.cpp \
    webdavfileinfo.cpp \
    webdavmodel.cpp

OTHER_FILES += qmldir

qmlfiles.files = $$files(*.qml) $$files(*.js)
qmlfiles.path = /opt/meedav/qml/$$uri
INSTALLS += qmlfiles

qmldir.files = qmldir
qmldir.path = /opt/meedav/qml/$$uri
INSTALLS += qmldir

target.path = /opt/meedav/qml/$$uri
INSTALLS += target
