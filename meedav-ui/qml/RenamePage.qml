import QtQuick 1.1
import com.nokia.meego 1.0

import "components"
import Webdav 1.0

Page {
    id: root

    property WebdavFileInfo item

    tools: topLevelTools

    PageHeader {
        id: appTitleRect
        text: "Rename"
    }

    Column {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 12
        spacing: 12

        Label {
            text: qsTr("Current name")
        }

        TextField {
            id: currentField
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: false
            text: item.displayName
        }

        Label {
            text: qsTr("Rename to")
        }

        TextField {
            id: inputField
            anchors.left: parent.left
            anchors.right: parent.right
            text: item.displayName
            placeholderText: qsTr("new name")
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: inputField.text != ""
            text: qsTr("Rename")
            onClicked: {
                webdavClient.rename(item.name, inputField.text);
                pageStack.pop();
            }
        }
    }
}
