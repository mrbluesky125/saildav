TEMPLATE = app
TARGET = meedav-ui

PACKAGEVERSION = $$system(head -n 1 ../qtc_packaging/debian_harmattan/changelog | grep -o [0-9].[0-9].[0-9])
DEFINES += "PACKAGEVERSION=\\\"$$PACKAGEVERSION\\\""

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

QT += sql declarative

CONFIG += link_pkgconfig \
        meegotouch

PKGCONFIG += qdeclarative-boostable

target.path = /opt/meedav/bin
INSTALLS += target

qmlfiles.files = $$files(qml/*.qml)
qmlfiles.path = /opt/meedav/qml
INSTALLS += qmlfiles

componentsfiles.files = $$files(qml/components/*.qml)
componentsfiles.path = /opt/meedav/qml/components
INSTALLS += componentsfiles

delegatesfiles.files = $$files(qml/delegates/*.qml)
delegatesfiles.path = /opt/meedav/qml/delegates
INSTALLS += delegatesfiles

imagesfiles.files = $$files(qml/images/*.png) $$files(qml/images/*.svg)
imagesfiles.path = /opt/meedav/qml/images
INSTALLS += imagesfiles

jsfiles.files = $$files(qml/js/*.js)
jsfiles.path = /opt/meedav/qml/js
INSTALLS += jsfiles

#splashfiles.files = splash_portrait.png
#splashfiles.path = /opt/meedav/meedav-ui
#INSTALLS += splashfiles

desktopfile.files = meedav_harmattan.desktop
desktopfile.path = /usr/share/applications
INSTALLS += desktopfile

icons.files = icon-l-meedav.png icon-m-meedav.png icon-l-meedav-application.svg
icons.path = /usr/share/themes/blanco/meegotouch/icons
INSTALLS += icons

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp 

HEADERS += 

OTHER_FILES += \
    qml/delegates/SectionDelegate.qml















