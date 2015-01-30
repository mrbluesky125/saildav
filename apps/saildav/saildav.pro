TARGET = harbour-saildav

#Include sub modules here
include($$modules_dir/qwebdav/qwebdav.pri)
include($$modules_dir/qquicktreemodel/qquicktreemodel.pri)
include($$qml_dir/org.bluesky.models/org.bluesky.models.pri)

#Include project files
INCLUDEPATH += $$PWD

#auto inclusion of all files in the (sub-)directory
SOURCES += main.cpp

HEADERS +=

RESOURCES += \ 
    qml/qml.qrc \
    silica/silica.qrc

OTHER_FILES += 

#additional Qt modules
QT += quick qml xml sql

#use c++11 compiler flags
CONFIG += c++11 

#Sailfish specific files
CONFIG += sailfishapp
OTHER_FILES += \
    translations/*.ts \
    harbour-saildav.desktop

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-saildav-de.ts

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += $$PWD/silica
QML_IMPORT_PATH += $$qml_dir
QML_IMPORT_PATH += $$plugin_dir
