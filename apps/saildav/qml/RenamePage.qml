import QtQuick 2.0
import QtQuick.Controls 1.0

import org.bluesky.basics 1.0
import org.bluesky.models 1.0
import qwebdav 1.0

Page {
    id: root

    property WebdavModel model

    Header {
        id: appTitleRect
        text: qsTr("Rename")
    }

    Column {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 12
        spacing: 12

        Label {
            text: qsTr("Rename to")
        }

        TextField {
            id: inputField
            anchors.left: parent.left
            anchors.right: parent.right
            text: model.currentItem.displayName
            placeholderText: qsTr("new name")
        }

        Button {
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: inputField.text != ""
            text: qsTr("Rename")
            onClicked: {
                model.rename(model.folder, inputField.text);
                stackView.pop();
            }
        }
    }
}
