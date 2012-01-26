import QtQuick 1.1
import com.nokia.meego 1.0
import "js/core.js" as Core

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    property variant accountPage: AccountPage {}
    property variant mainPage: MainPage {}

    ToolBarLayout {
        id: commonTools
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: webdavClient.currentPath = webdavClient.currentPath + "../"
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
                text: qsTr("Account")
                onClicked: appWindow.pageStack.push(appWindow.accountPage, {account: Core.getAccount()});
            }
            MenuItem {
                text: qsTr("About")
                onClicked: appWindow.pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }
    }

    Component.onCompleted:
    {
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
