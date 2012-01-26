import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    anchors.margins: 6
    width: parent.width
    height: 64

    Rectangle {
        id: background
        anchors.fill: parent
        color: "lightsteelblue";
        opacity: 0.0

        Behavior on opacity { NumberAnimation {} }
    }

    Row {
        anchors.fill: parent
        spacing: 6
        Image {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.height
            source: ""
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: name
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: background.opacity = 1.0;
        onReleased: background.opacity = 0.0;
        onPositionChanged: background.opacity = 0.0;
        onClicked: webdavClient.currentPath = name;
    }
}
