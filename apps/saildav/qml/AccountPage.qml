import QtQuick 2.0
import QtQuick.Controls 1.1

import "components"
import "js/core.js" as Core

Page {
    id: root

    property variant account: Core.defaultAccount()

    tools: topLevelTools

    PageHeader {
        id: appTitleRect
        text: "MeeDav"
    }

    Flickable {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentHeight: childrenRect.height

        Column {
            id: contentColumn

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 12
            spacing: 6

            Label {
                text: "Username"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: usernameField
                text: account.username
            }

            Label {
                text: "Password"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: passwordField
                text: account.password
                echoMode: TextInput.Password
            }

            Label {
                text: "Server/URL"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: urlField
                text: account.url
            }

            Label {
                text: " "
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Save"
                onClicked: {
                    var updateAccount =  {id: account.id, username: usernameField.text, password: passwordField.text, url: urlField.text }
                    Core.updateAccount( updateAccount );
                    pageStack.replace(Qt.resolvedUrl("WebdavPage.qml"), {account: Core.getAccount()});
                }
            }
        }
    }
}
