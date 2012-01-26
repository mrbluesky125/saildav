import QtQuick 1.1
import com.nokia.meego 1.0
import "js/core.js" as Core

PageStackWindow {
    id: appWindow

    initialPage: mainPage

<<<<<<< HEAD
    property variant accountPage: AccountPage {}
    property variant mainPage: MainPage {}
=======
    MainPage {
        id: mainPage
    }
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a

    ToolBarLayout {
        id: commonTools
        visible: true
<<<<<<< HEAD

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
=======
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
        }
    }

    Menu {
<<<<<<< HEAD
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
=======
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
        }
    }

    Component.onCompleted:
    {
        Core.openDB();
<<<<<<< HEAD
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
=======
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
    }
}
