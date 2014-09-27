import QtQuick 2.0
import QtQuick.Controls 1.1

import "delegates"

import org.bluesky.basics 1.0
import org.bluesky.models 1.0
import qwebdav 1.0

Page {
    id: webdavPage

//    tools: ToolBarLayout {
//        ToolIcon {
//            platformIconId: "toolbar-back"
//            onClicked: webdavClient.folder = webdavClient.parentFolder
//        }
//        ToolIcon {
//            platformIconId: "toolbar-home"
//            onClicked: webdavClient.cd(webdavClient.homePath)
//        }
//        ToolIcon {
//            platformIconId: "toolbar-refresh"
//            onClicked: webdavClient.refresh()
//        }
//        ToolIcon {
//            platformIconId: "toolbar-view-menu"
//            onClicked: (menu.status === DialogStatus.Closed) ? menu.open() : menu.close()
//        }
//    }

//    Menu {
//        id: menu
//        MenuLayout {
//            MenuItem {
//                text: qsTr("New Folder")
//                onClicked: pageStack.push(Qt.resolvedUrl("CreateFolderPage.qml"), { model: webdavClient });
//            }
//            MenuItem {
//                text: qsTr("Upload")
//                onClicked: pageStack.push(Qt.resolvedUrl("FilesystemPage.qml"), { model: webdavClient });
//            }
//            MenuItem {
//                text: qsTr("Account")
//                onClicked: pageStack.replace(Qt.resolvedUrl("AccountPage.qml"), { account: Core.getAccount() });
//            }
//            MenuItem {
//                text: qsTr("About")
//                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
//            }
//        }
//    }

//    Menu {
//        id: itemMenu
//        visualParent: pageStack

//        property WebdavFileInfo item: WebdavFileInfo { }

//        MenuLayout {
//            MenuItem {
//                text: qsTr("Download")
//                enabled: !itemMenu.item.dir && !itemMenu.item.busy
//                onClicked: webdavClient.download(itemMenu.item.name);
//            }
//            MenuItem {
//                text: qsTr("Rename")
//                onClicked: pageStack.push(Qt.resolvedUrl("RenamePage.qml"), { model: webdavClient });
//                enabled: !itemMenu.item.busy
//            }
//            MenuItem {
//                text: qsTr("Delete")
//                onClicked: webdavClient.remove(itemMenu.item.name)
//                enabled: !itemMenu.item.busy
//            }
//            MenuItem {
//                text: qsTr("Abort")
//                onClicked: itemMenu.item.abort();
//                enabled: itemMenu.item.busy
//            }
//        }
//    }

    onStackStatusChanged: {
        if(stackStatus === Stack.Activating)
            account = accountModel.get(0);
    }

    property variant account

    SqlLiteModel {
        id: accountModel
        databaseName: "saildav"
        databaseVersion: "1.0"
        tableName: "accounts"
        primaryKeyName: "id"
    }

    Header {
        id: appTitleRect
        text: webdavClient.folder == webdavClient.homePath ? "Home" : webdavClient.currentItem.displayName
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: webdavClient.currentItem.busy
        opacity: webdavClient.currentItem.busy ? 1.0 : 0.0

        Behavior on opacity { NumberAnimation {} }
    }

    ListView {
        id: itemView
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        spacing: 10
        clip: true

        section.property: "displayName"
        section.criteria: ViewSection.FirstCharacter
        section.delegate: SectionDelegate { }

        delegate: WebdavUrlInfoDelegate { }
        model: webdavClient

        function delegateClicked(index) {
            model.folder = model[index].name;
        }

        function delegatePressAndHold(index) {
            itemMenu.item = model[index];
            itemMenu.open();
        }
    }

    WebdavModel {
        id: webdavClient
        userName: account.username
        password: account.password
        baseUrl: account.url
        onErrorChanged: showError(error);
    }

    //Component.onCompleted: webdavClient.folder = webdavClient.homePath
}
