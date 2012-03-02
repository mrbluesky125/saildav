import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "js/core.js" as Core

import Webdav 1.0

PageStackWindow {
    id: appWindow

    ToolBarLayout {
        id: topLevelTools
        visible: false

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }  

    Component.onCompleted: {
        Core.openDB();
        if(Core.getAccount()["url"] === undefined) {
            Core.createAccount(Core.defaultAccount());
            pageStack.push(Qt.resolvedUrl("AccountPage.qml"), {account: Core.getAccount()});
        }
        else {
            var account = Core.getAccount();
            pageStack.push(Qt.resolvedUrl("WebdavPage.qml"), {account: Core.getAccount()});
        }
    }


}
