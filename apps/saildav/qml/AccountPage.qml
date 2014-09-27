import QtQuick 2.0
import QtQuick.Controls 1.1

import org.bluesky.basics 1.0
import org.bluesky.models 1.0

Page {
    id: root

    SqlLiteModel {
        id: accountModel
        databaseName: "saildav"
        databaseVersion: "1.0"
        tableName: "accounts"
        primaryKeyName: "id"
    }

    Header {
        id: appTitleRect
        text: "SailDav - " + qsTr("New Account")
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
            spacing: 12

            Label {
                text: "Username"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: usernameField
                text: accountModel.get(0).username
            }

            Label {
                text: "Password"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: passwordField
                text: accountModel.get(0).password
                echoMode: TextInput.Password
            }

            Label {
                text: "Server/URL"
            }

            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                id: urlField
                text: accountModel.get(0).url
            }

            Label {
                text: " "
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Save"
                onClicked: {
                    var updateAccount =  { username: usernameField.text, password: passwordField.text, url: urlField.text }
                    accountModel.setSql(0, updateAccount);
                    stackView.replace(Qt.resolvedUrl("WebdavPage.qml"), { account: updateAccount });
                }
            }
        }
    }
}
