import QtQuick 2.0
import Sailfish.Silica 1.0

import org.bluesky.models 1.0
import components 1.0
import webdav 1.0

Page {
    id: page

    property var account

    WebdavModel {
        id: webdavClient
        userName: account.username
        password: account.password
        baseUrl: account.url
        onErrorChanged: showError(error);
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: webdavClient.currentItem.childs

        header: PageHeader {
            title: webdavClient.folder === webdavClient.homePath ? "Home" : webdavClient.currentItem.displayName
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: qsTr("Ordner ist leer")
            hintText: webdavClient.homePath
        }

        PullDownMenu {
            busy: webdavClient.currentItem.busy
            MenuItem {
                text: qsTr("Über SailDAV")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("Aktualisieren")
                onClicked: {
                    webdavClient.refresh();
                }
            }
            MenuItem {
                text: "Home"
                onClicked: {
                    webdavClient.folder = webdavClient.homePath
                }
            }
            MenuItem {
                text: webdavClient.currentItem.parentItem.name
                onClicked: {
                    webdavClient.folder = webdavClient.parentFolder
                }
            }
        }

        delegate: ListItem {

            menu: ContextMenu {
                MenuItem {
                    text: "Download"
                    onClicked: {
                        remorseAction("Deleting", function() { accountsModel.removeSql(rowid); });
                    }
                }
                MenuItem {
                    text: "Delete"
                    onClicked: {
                        remorseAction("Deleting", function() {  });
                    }
                }
            }
            onClicked: {
                if(dir === true)
                    webdavClient.folder = name
            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                text: displayName
                color: highlighted ? Theme.highlightColor : Theme.primaryColor
                anchors.verticalCenter: parent.verticalCenter
            }

        }
        VerticalScrollDecorator {}
    }
}
