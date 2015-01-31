import QtQuick 2.1
import Sailfish.Silica 1.0

Page {
    id: pageRoot

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Choose server")
            }

            TextField {
                id: url
                width: parent.width
                validator: RegExpValidator { regExp: /^[http:\/\/,https:\/\/].*/ }

                placeholderText: qsTr("URL (e.g. http://my.server.com/dav)")
                label: placeholderText
                text: "http://"
            }

            TextField {
                id: username
                width: parent.width

                inputMethodHints: Qt.ImhNoAutoUppercase
                placeholderText: qsTr("User")
                label: placeholderText
            }

            TextField {
                id: password
                width: parent.width

                echoMode: TextInput.Password
                placeholderText: qsTr("Password")
                label: placeholderText
            }

            TextSwitch {
                id: autoLoginSwitch
                visible: false
                text: qsTr("Auto Login")
                checked: false
            }

            TextSwitch {
                id: certSwitch
                visible: false
                text: qsTr("Ignoriere Zertifikatsfehler")
                checked: false
            }

            Button {
                id: continueButton
                //enabled: url.valid
                text: qsTr("Add")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    accountsModel.insertSql({ "url": url.text, "username": username.text, "password": password.text });
                    pageStack.pop();
                }
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: false
            opacity: running ? 1.0 : 0.0
        }
    }
}
