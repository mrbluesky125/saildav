# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = examplesailfishapp

CONFIG += sailfishapp

SOURCES += src/examplesailfishapp.cpp

OTHER_FILES += qml/examplesailfishapp.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/examplesailfishapp.changes.in \
    rpm/examplesailfishapp.spec \
    rpm/examplesailfishapp.yaml \
    translations/*.ts \
    examplesailfishapp.desktop

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/examplesailfishapp-de.ts

