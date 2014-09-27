#copy the executable to the source folder
win32:DLLDESTDIR = $$_PRO_FILE_PWD_

#Include sub modules here
include($$modules_dir/qwebdav/qwebdav.pri)
include($$modules_dir/qquicktreemodel/qquicktreemodel.pri)
include($$qml_dir/org.bluesky.basics/org.bluesky.basics.pri)
include($$qml_dir/org.bluesky.models/org.bluesky.models.pri)

#Include project files
INCLUDEPATH += $$PWD

#auto inclusion of all files in the (sub-)directory
SOURCES += main.cpp

HEADERS +=

RESOURCES += \ 
    qml/qml.qrc

OTHER_FILES += 

#additional Qt modules
QT += quick qml xml

#use c++11 compiler flags
CONFIG += c++11 

#additional libraries (configured with the feature files in the qmake folder)
#CONFIG += opencv libs

#Android specific variables - will be ignored in other configurations
ANDROID_PACKAGE_SOURCE_DIR =
ANDROID_EXTRA_LIBS =
OTHER_FILES += $$files($$PWD/android/*)

#Sailfish specific files
CONFIG += sailfishapp
OTHER_FILES += \
    rpm/saildav.changes \
    rpm/saildav.spec \
    rpm/saildav.yaml \
    translations/*.ts \
    saildav.desktop

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/saildav-de.ts

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += $$qml_dir
QML_IMPORT_PATH += $$plugin_dir
