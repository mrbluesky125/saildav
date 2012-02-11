import QtQuick 1.1
import com.nokia.meego 1.0

import Webdav 1.0

Menu {
    id: itemMenu
    visualParent: pageStack

    property PageStack pageStack
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
            onClicked: itemMenu.pageStack.push(Qt.resolvedUrl("RenamePage.qml"), {item: itemMenu.item});
        }
        MenuItem {
            text: qsTr("Delete")
            onClicked: webdavClient.remove(itemMenu.item.name)
        }
    }
}
