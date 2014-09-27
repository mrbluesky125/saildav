import QtQuick 2.0
import QtQuick.Controls 1.1

import "../components"

Item {
    id: root
    width: parent.width
    height: 80

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
        color: "gray";
        opacity: 0.0

        Behavior on opacity { NumberAnimation {} }
    }

    ProgressBar {
        id: progressBackground

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        itemProgress: progress
        running: busy
        color: "#32c81e"
    }

    Row {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 10

        Item {
            id: logo
            height: parent.height
            width: parent.height

            Image {
                anchors.centerIn: parent
                height: parent.height
                width: parent.height
                source: dir ? "../images/icon-m-common-directory.png" : "../images/icon-l-sharing-document.png"
                smooth: true
                opacity: busy ? 0.1 : 1.0
                cache: false

            }

            BusyIndicator {
                id: busyIndicator
                anchors.centerIn: parent
                visible: busy
                running: true
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 6
            spacing: 2

            Label {
                text: displayName !== "" ? displayName : "UNDEFINED"
                font.pixelSize: 26
                font.family: "Nokia Pure Bold"
            }

            Label {
                text: mimeType
                font.pixelSize: 14
            }

            Label {
                text: lastModified + (file ? (" - " + sizeString()) : "")
                font.pixelSize: 14
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: background.opacity = 1.0;
        onReleased: background.opacity = 0.0;
        onPositionChanged: background.opacity = 0.0;
        onClicked: root.ListView.view.delegateClicked(index)
        onPressAndHold: root.ListView.view.delegatePressAndHold(index)
    }
}
