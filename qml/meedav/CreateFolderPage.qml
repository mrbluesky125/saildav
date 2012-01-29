import QtQuick 1.1
import com.nokia.meego 1.0

import MeeDav 1.0

Page {
    id: root

    property WebdavUrlInfo item

    tools: topLevelTools

    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        Label {
            text: "Create a new folder in"
        }

        TextField {
            id: currentField
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: false
            text: item.name
        }

        Label {
            text: "New folder"
        }

        TextField {
            id: inputField
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "Folder name"
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: inputField.text != ""
            text: "Create folder"
            onClicked: {
                webdavClient.mkdir(currentField.text + inputField.text + "/");
                pageStack.pop();
            }
        }
    }
}
