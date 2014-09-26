import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: parent.width
    height: 80

    property bool dir: ListView.view.model.isFolder(index)

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

    Row {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 10

        Image {
            id: logo
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: parent.height
            source: dir ? "../images/icon-m-common-directory.png" : "../images/icon-l-sharing-document.png"
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: 6
            spacing: 3

            Label {
                text: fileName
                font.pixelSize: 26
                font.family: "Nokia Pure Bold"
            }
        }
    }

    ToolIcon {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 6
        iconId: "transfer-upload"
        visible: !dir
        onClicked: root.ListView.view.upload(filePath)
    }

    MouseArea {
        anchors.fill: parent
        onPressed: background.opacity = 1.0;
        onReleased: background.opacity = 0.0;
        onPositionChanged: background.opacity = 0.0;
        onClicked: {
            if(dir) root.ListView.view.model.folder = filePath;
        }
    }
}
