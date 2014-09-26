#copy the executable to the source folder
DLLDESTDIR = $$_PRO_FILE_PWD_

#Include sub modules here - examples:
include($$modules_dir/examplemodule/examplemodule.pri)         #example module
include($$qml_dir/org.emblab.example/org.emblab.example.pri)   #example qml components

#Include project files
INCLUDEPATH += $$PWD

#auto inclusion of all files in the (sub-)directory
SOURCES += main.cpp

HEADERS +=

RESOURCES += 

OTHER_FILES += 

#additional Qt modules
QT += quick qml

#use c++11 compiler flags
CONFIG += c++11 

#additional libraries (configured with the feature files in the qmake folder) - libs has to be at the end of the config var
#CONFIG += opencv libs

#Android specific variables - will be ignored in other configuration
ANDROID_PACKAGE_SOURCE_DIR =
ANDROID_EXTRA_LIBS =
OTHER_FILES += $$files($$PWD/android/*)

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH += $$qml_dir
QML_IMPORT_PATH += $$plugin_dir
