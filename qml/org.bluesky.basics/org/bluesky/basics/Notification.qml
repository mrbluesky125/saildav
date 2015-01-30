import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0

import "."

Item {
    id: root

    width: parent.width
    height: layout.height + (LayoutSettings.standardMargin * 2)

    visible: opacity > 0.0

    property alias text: captionLabel.text
    property alias timeout: hideTimer.interval
    property alias description: subCaptionLabel.text
    property alias iconSource: icon.source
    property alias color: background.color
    property alias font: captionLabel.font

    signal opened
    signal closed

    function show() {
        hideTimer.start();
    }

    function hide() {
        hideTimer.stop();
    }

    layer.enabled: true
    layer.effect: DropShadow {
        radius: 12
        samples: 16
        source: root
        horizontalOffset: 20
        verticalOffset: 20
        color: Qt.rgba(0, 0, 0, 0.5)
        transparentBorder: true
    }

    Timer {
        id: hideTimer
        interval: 3000
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
        opacity: 0.8
        color: "lightblue"
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
        id: layout
        width: parent.width
        height: childrenRect.height
        anchors.margins: LayoutSettings.standardMargin

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
            font.pointSize: LayoutSettings.font.itemPointSize
            renderType: Text.QtRendering
        }

        Label {
            id: subCaptionLabel
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            font.pointSize: LayoutSettings.font.subItemPointSize
            renderType: Text.QtRendering
        }
    }

    MouseArea {
        anchors.fill: parent
        preventStealing: true
        onClicked: root.hide()
    }

    Item {
        id: stateMachine
        state: root.state
        states: [
            State {
                name: "ERROR"
                PropertyChanges {
                    target: icon
                    source: "icons/error_icon.png"
                }
                PropertyChanges {
                    target: background
                    color: "red"
                }
            }

        ]
    }
}
