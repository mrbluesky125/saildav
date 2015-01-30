import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: accountsModel

        header: PageHeader {
            title: qsTr("Server auswählen")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Über SailDAV")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("Server hinzufügen")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AddAccountPage.qml"))
                }
            }
        }

        delegate: BackgroundItem {
            width: listView.width
            Label {
                id: firstName
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                text: model.title
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                anchors.verticalCenter: parent.verticalCenter
            }
            onClicked: {}
        }
        VerticalScrollDecorator {}
    }
}


