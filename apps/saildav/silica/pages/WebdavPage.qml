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
            text: qsTr("Directory empty")
            hintText: webdavClient.homePath
        }

        PullDownMenu {
            busy: webdavClient.currentItem.busy
            MenuItem {
                text: qsTr("About SailDAV")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    webdavClient.refresh();
                }
            }
            MenuItem {
                text: qsTr("Home")
                visible: webdavClient.folder !== webdavClient.homePath
                onClicked: {
                    webdavClient.folder = webdavClient.homePath
                }
            }
            MenuItem {
                text: qsTr("Back")
                enabled: webdavClient.folder !== webdavClient.homePath
                onClicked: {
                    webdavClient.folder = webdavClient.parentFolder
                }
            }
        }

        PushUpMenu {
            busy: webdavClient.currentItem.busy
            MenuItem {
                text: qsTr("Create folder")
                enabled: false
                onClicked: {

                }
            }
            MenuItem {
                text: qsTr("Upload")
                enabled: false
                onClicked: {

                }
            }
        }

        delegate: ListItem {

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Download")
                    visible: !dir
                    enabled: !busy
                    onClicked: {
                        remorseAction(qsTr("Downloading"), function() { webdavClient.download(name) });
                    }
                }
                MenuItem {
                    text: qsTr("Delete")
                    enabled: !busy
                    onClicked: {
                        remorseAction(qsTr("Deleting"), function() { webdavClient.remove(name) });
                    }
                }
                MenuItem {
                    text: qsTr("Abort")
                    enabled: busy
                    onClicked: {
                        remorseAction(qsTr("Abort"), function() { webdavClient.abort() });
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
