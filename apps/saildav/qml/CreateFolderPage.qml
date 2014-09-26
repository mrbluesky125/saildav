import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

import Webdav 1.0

Page {
    id: root

    property WebdavModel model

    tools: topLevelTools

    PageHeader {
        id: appTitleRect
        text: "New folder"
    }

    Column {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
            text: model.folder
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
                model.mkdir(currentField.text + inputField.text + "/");
                pageStack.pop();
            }
        }
    }
}
