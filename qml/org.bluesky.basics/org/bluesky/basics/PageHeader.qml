import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0

import "."

Rectangle {
    id: root

    property alias text: headerText.text
    property alias font: headerText.font
    property alias backButton: backButtonItem

    anchors.top: parent.top
    width: parent.width
    height: layout.height + LayoutSettings.standardMargin*2
    color: "#F5F5F7"

    signal backClicked

    RowLayout {
        id: layout
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        spacing: LayoutSettings.standardMargin

        Item {
            id: backButtonItem
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            height: headerText.height
            width: height

            Image {
                id: backButtonImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "icons/back_icon.png"
                smooth: true
                scale: backMouseArea.pressed ? 0.9 : 1.0
            }

            MouseArea {
                id: backMouseArea
                anchors.fill: parent
                onClicked: root.backClicked()
            }
        }

        Label {
            id: headerText
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Qt.AlignVCenter
            elide: Text.ElideMiddle
            font.bold: true
            font.pointSize: LayoutSettings.font.headerPointSize
            renderType: Text.QtRendering
            color: "#555"

            Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.InOutElastic } }
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        anchors.bottom: parent.bottom
        color: "#DCDCDE"
    }
}
