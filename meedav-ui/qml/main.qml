import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "js/core.js" as Core

import Webdav 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    property variant accountPage: AccountPage {}
    property variant mainPage: MainPage {}

    function showError(msg) {
        banner.text = msg;
        iconSource: "image://theme/icon-l-error"
        banner.show();
    }

    InfoBanner {
        id: banner
        y: 50
    }

    WebdavModel {
        id: webdavClient
        onErrorChanged: showError(error);
    }

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: webdavClient.folder = webdavClient.parentFolder
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
            onClicked: (menu.status === DialogStatus.Closed) ? menu.open() : menu.close()
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
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("CreateFolderPage.qml"), {item: webdavClient.currentItem});
            }
            MenuItem {
                text: qsTr("Upload")
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("FilesystemPage.qml"));
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

    ItemMenu {
        id: itemMenu
        visualParent: pageStack

        property WebdavFileInfo item: WebdavFileInfo { }

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
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("RenamePage.qml"), {item: itemMenu.item});
            }
            MenuItem {
                text: qsTr("Delete")
                onClicked: webdavClient.remove(itemMenu.item.name)
            }
        }
    }

    Menu {
        id: busyMenu
        visualParent: pageStack

        property WebdavFileInfo item: WebdavFileInfo { }

        MenuLayout {
            MenuItem {
                text: qsTr("Abort")
                onClicked: busyMenu.item.abort();
            }
        }
    }

    Menu {
        id: uploadMenu
        visualParent: pageStack

        property string filePath: ""

        MenuLayout {
            MenuItem {
                text: qsTr("Upload")
                onClicked: {
                    webdavClient.upload(webdavClient.currentItem.name, uploadMenu.filePath);
                    pageStack.pop();
                }
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
