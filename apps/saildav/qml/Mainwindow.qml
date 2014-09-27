import QtQuick 2.0
import QtQuick.Controls 1.1

import "js/core.js" as Core

import qwebdav 1.0

Item {
    id: appWindow


    StackView {
       anchors.fill: parent

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
