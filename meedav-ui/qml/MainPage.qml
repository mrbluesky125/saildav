import QtQuick 1.1
import com.nokia.meego 1.0
import "components"
import "delegates"

import Webdav 1.0

Page {
    tools: commonTools

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
    }

    PageHeader {
        id: appTitleRect
        text: webdavClient.folder == webdavClient.homePath ? "Home" : webdavClient.currentItem.displayName
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: webdavClient.currentItem.busy
        opacity: webdavClient.currentItem.busy ? 1.0 : 0.0

        Behavior on opacity { NumberAnimation {} }

        platformStyle: BusyIndicatorStyle {
            size: "large"
        }
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
        model: webdavClient.currentItem.childs
    }


    ScrollDecorator {
        flickableItem: itemView
        platformStyle: ScrollDecoratorStyle {

        }
    }
}
