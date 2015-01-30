import QtQuick 2.1
import Sailfish.Silica 1.0

import org.bluesky.models 1.0
import components 1.0

Page {
    id: page

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: accountsModel

        header: PageHeader {
            title: qsTr("Server auswählen")
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("Serverliste ist leer")
            hintText: qsTr("Füge einen WebDAV-Account über das Pull-Down-Menü hinzu")
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

        delegate: ListItem {

            menu: ContextMenu {
                MenuItem {
                    text: "Delete"
                    onClicked: {
                        remorseAction("Deleting", function() { accountsModel.removeSql(rowid); });
                    }
                }
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                label: url
                text: username
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                anchors.verticalCenter: parent.verticalCenter
                separator: index < (accountsModel.count-1)
            }
            onClicked: {
                pageStack.push(Qt.resolvedUrl("WebdavPage.qml"), { "account": accountsModel.get(index) });
            }
        }
        VerticalScrollDecorator {}
    }
}


