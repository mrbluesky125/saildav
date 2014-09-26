import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    id: root

    width: parent.width
    height: 70

    visible: opacity > 0.0
    Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }

    property alias text: captionLabel.text
    property alias timeout: liveTimer.interval
    property alias description: subCaptionLabel.text
    property alias iconSource: icon.source
    property alias color: background.color
    property alias font: captionLabel.font

    signal opened
    signal closed

    function show() {
        liveTimer.start();
    }

    function hide() {
        liveTimer.stop();
    }

    Timer {
        id: liveTimer
        interval: 5000
        repeat: false
        triggeredOnStart: false
        onRunningChanged: {
            if(running) root.opened()
            else root.closed()
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        opacity: 0.4
        color: "lightblue"
        radius: 12
        antialiasing: true
    }

    Rectangle {
        id: backgroundBorder
        anchors.fill: parent
        color: "transparent"
        border.width: 3
        border.color: background.color
        radius: background.radius
        antialiasing: true
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8

        rows: 2
        columns: 2

        Item {
            id: iconWrapper
            height: parent.height
            width: height
            Layout.rowSpan: 2

            Image {
                id: icon
                anchors.fill: parent
                smooth: true
                fillMode: Image.PreserveAspectFit
                source: "icons/info_icon.png"
            }
        }

        Label {
            id: captionLabel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            font.pointSize: 18
            renderType: Text.QtRendering
        }

        Label {
            id: subCaptionLabel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            font.pointSize: captionLabel.font.pointSize - 6
            font.family: captionLabel.font.family
            renderType: Text.QtRendering
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.hide()
    }
}
