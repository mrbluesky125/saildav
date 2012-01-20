import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: root

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
        height: 56
        width: 56
    }

    Label {
        id: appTitle
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: appIcon.right
        anchors.leftMargin: 12
        font.pixelSize: 30
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
        source: "image://theme/meegotouch-separator-inverted-background-horizontal"
    }
}
