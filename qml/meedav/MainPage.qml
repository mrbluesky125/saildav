import QtQuick 1.1
import com.nokia.meego 1.0
import "components"
import "delegates"

Page {
    tools: commonTools

    PageHeader {
        id: appTitleRect
        text: "MeeDav"
    }

    ListView {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        clip: true

        delegate: WebdavUrlInfoDelegate { }
        model: webdavClient.currentItem.items
    }

}
