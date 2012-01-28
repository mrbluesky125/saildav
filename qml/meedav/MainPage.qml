import QtQuick 1.1
import com.nokia.meego 1.0
import "components"
import "delegates"

Page {
    tools: commonTools

    PageHeader {
        id: appTitleRect
        text: webdavClient.currentItem.name == webdavClient.homePath ? "Home" : webdavClient.currentItem.displayName
        busy: webdavClient.currentItem.busy
    }

    ListView {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        spacing: 10
        clip: true

        section.property: "displayName"
        section.criteria: ViewSection.FirstCharacter

        delegate: WebdavUrlInfoDelegate { }
        model: webdavClient.currentItem.childs
    }
}
