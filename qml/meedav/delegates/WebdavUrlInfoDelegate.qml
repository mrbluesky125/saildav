import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: parent.width
    height: 100

    function sizeString() {
        if(size > 1024*1024*1024)
            return Math.floor(size/(1024*1024*1024)) + " Gb"
        else if(size > 1024*1024)
            return Math.floor(size/(1024*1024)) + " Mb"
        else if(size > 1024)
            return Math.floor(size/1024) + " Kb"
        else
            return size + " Bytes"
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "darkgray";
        opacity: 0.0

        Behavior on opacity { NumberAnimation {} }
    }

    Rectangle {
        id: progressBackground

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width*progress

        color: "#32c81e"
        opacity: busy ? 0.5 : 0.0

        Behavior on width { NumberAnimation {} }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 12
        opacity: busy ? 1.0 : 0.0
        running: busy
    }

    Row {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 10
        Image {
            id: logo
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.height
            source: dir ? "image://theme/icon-m-common-directory" : "image://theme/icon-l-sharing-document"
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 6
            spacing: 3

            Label {
                text: displayName
                font.pixelSize: 32
                font.family: "Nokia Pure Bold"
            }

            Label {
                text: mimeType
                font.pixelSize: 16
            }

            Label {
                text: lastModified + (file ? (" - " + sizeString()) : "")
                font.pixelSize: 16
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: background.opacity = 1.0;
        onReleased: background.opacity = 0.0;
        onPositionChanged: background.opacity = 0.0;
        onClicked: {
            if(busy) return;
            if(dir) webdavClient.cd(name);
        }
        onPressAndHold: {
            if(busy) return;
            itemMenu.item = root.ListView.view.model[index]
            itemMenu.open();
        }
    }
}
