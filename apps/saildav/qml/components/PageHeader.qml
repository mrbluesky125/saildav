import QtQuick 2.0
import QtQuick.Controls 1.1

Rectangle {
    id: root

    property bool iconEnabled: false
    property alias text: appTitle.text
    property alias iconSource: appIcon.source
    property alias busy: busyIndicator.running

    height: 70
    color: "orange"
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    Image {
        id: appIcon
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 12
        height: iconEnabled ? 56 : 0
        width: height
        smooth: true
        source: "../images/meedav.svg"
    }

    Label {
        id: appTitle
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: appIcon.right
        anchors.leftMargin: 12
        font.pixelSize: 30
        color: "white"
    }

    BusyIndicator {
        id: busyIndicator
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 12
        opacity: running ? 1.0 : 0.0
        running: false
    }

    Image {
        id: bottomLine
        anchors.bottom: parent.bottom
        width: parent.width
        height: 2
        source: "image://theme/meegotouch-separator-background-horizontal"
    }
}
