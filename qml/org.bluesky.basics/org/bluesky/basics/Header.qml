import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: root
    property alias text: headerText.text
    property alias font: headerText.font
    property alias backButton: backButtonItem

    anchors.top: parent.top
    width: parent.width
    height: 70 * (Screen.pixelDensity / 2.5) //initial height - can be changed
    color: "#F5F5F7"

    signal backClicked

    RowLayout {
        id: layout
        anchors.fill: parent
        anchors.leftMargin: 12
        spacing: 12

        Item {
            id: backButtonItem
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            height: parent.height * 0.50
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
            font.pointSize: 26
            fontSizeMode: Text.VerticalFit
            renderType: Text.QtRendering
            color: "#555"
        }

    }

    Rectangle {
        width: parent.width
        height: 1
        anchors.top: parent.bottom
        color: "#DCDCDE"
    }

    LinearGradient {
        width: parent.width
        anchors.top: parent.bottom
        height: 18

        start: Qt.point(0, 0)
        end: Qt.point(0, height)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#7FDCDCDE" }
            GradientStop { position: 1.0; color: "#00DCDCDE" }
        }
    }
}
