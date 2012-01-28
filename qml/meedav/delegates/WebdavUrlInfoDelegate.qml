import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    anchors.margins: 6
    width: parent.width
    height: 100

    Rectangle {
        id: background
        anchors.fill: parent
        color: "lightsteelblue";
        opacity: 0.0

        Behavior on opacity { NumberAnimation {} }
    }

    Row {
        anchors.fill: parent
        spacing: 10
        Image {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.height
            source: dir ? "image://theme/icon-m-common-directory" : ""
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: displayName
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: background.opacity = 1.0;
        onReleased: background.opacity = 0.0;
        onPositionChanged: background.opacity = 0.0;
        onClicked: {
            if(dir)
                webdavClient.cd(name);
            else if(file)
                webdavClient.download(name);
        }
    }
}
