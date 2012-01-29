import QtQuick 1.1
import com.nokia.meego 1.0

import "js/core.js" as Core

import MeeDav 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    property variant createFolderPage: CreateFolderPage { }
    property variant accountPage: AccountPage {}
    property variant mainPage: MainPage {}


    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: webdavClient.cd("..")
        }

        ToolIcon {
            platformIconId: "toolbar-home"
            onClicked: webdavClient.cd(webdavClient.homePath)
        }

        ToolIcon {
            platformIconId: "toolbar-refresh"
            onClicked: webdavClient.refresh()
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }
    }

    ToolBarLayout {
        id: topLevelTools
        visible: false

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: appWindow.pageStack.pop()
        }
    }

    Menu {
        id: menu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("New Folder")
                onClicked: appWindow.pageStack.push(appWindow.createFolderPage, {currentFolder: webdavClient.currentItem.name});
            }
            MenuItem {
                text: qsTr("Upload")
                enabled: false
                onClicked: appWindow.pageStack.push(appWindow.createFolderPage, {currentFolder: webdavClient.currentItem.name});
            }
            MenuItem {
                text: qsTr("Account")
                onClicked: appWindow.pageStack.push(appWindow.accountPage, {account: Core.getAccount()});
            }
            MenuItem {
                text: qsTr("About")
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    Menu {
        id: itemMenu
        visualParent: pageStack

        property WebdavUrlInfo item: WebdavUrlInfo { }

        MenuLayout {
            MenuItem {
                text: qsTr("Download")
                enabled: !itemMenu.item.dir
                onClicked: webdavClient.download(itemMenu.item.name);
            }
            MenuItem {
                text: qsTr("Copy")
                enabled: false
                onClicked: ;
            }
            MenuItem {
                text: qsTr("Move")
                enabled: false
                onClicked: ;
            }
            MenuItem {
                text: qsTr("Rename")
                enabled: false
                onClicked: ;
            }
            MenuItem {
                text: qsTr("Delete")
                onClicked: webdavClient.remove(itemMenu.item.name)
            }
        }
    }

    Component.onCompleted: {
        Core.openDB();
        if(Core.getAccount()["url"] === undefined) {
            Core.createAccount(Core.defaultAccount());
            appWindow.pageStack.push(appWindow.accountPage, {account: Core.getAccount()});
        }
        else {
            var account = Core.getAccount();
            webdavClient.userName = account.username;
            webdavClient.password = account.password;
            webdavClient.baseUrl = account.url;
        }
    }


}
