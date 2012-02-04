import QtQuick 1.1
import com.nokia.meego 1.0

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

        Image {
            id: logo
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.height
            source: dir ? "../images/icon-m-common-directory.png" : "../images/icon-l-sharing-document.png"
            smooth: true
            cache: false
            transform: Translate {
                id: logoTrans
                y: 0

                SequentialAnimation on y {
                    id: logoAnimation
                    running: false
                    loops: Animation.Infinite

                    NumberAnimation {
                        target: logoTrans
                        property: "y"
                        duration: 750
                        easing.type: Easing.OutQuad
                        from: 0
                        to: -10
                    }
                    NumberAnimation {
                        target: logoTrans
                        property: "y"
                        duration: 1000
                        easing.period: 0.5
                        easing.type: Easing.OutElastic
                        from: -10
                        to: 0
                    }
                }
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

    states: State {
        name: "BUSY"
        when: busy
        PropertyChanges { target: logo; source: "../images/icon-m-email-directory-outgoing.png" }
        PropertyChanges { target: logoAnimation; running: true }
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
            if(busy) {
                busyMenu.item = root.ListView.view.model[index]
                busyMenu.open();
            }
            else {
                itemMenu.item = root.ListView.view.model[index]
                itemMenu.open();
            }
        }
    }
}
