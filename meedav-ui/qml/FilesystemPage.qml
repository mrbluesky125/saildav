import QtQuick 1.1
import com.nokia.meego 1.0
import Qt.labs.folderlistmodel 1.0
import "components"
import "delegates"

Page {
    tools: ToolBarLayout {
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: folderModel.folder = folderModel.parentFolder
        }
        ToolIcon {
            platformIconId: "toolbar-close"
            onClicked: appWindow.pageStack.pop()
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
    }

    PageHeader {
        id: appTitleRect
        text: folderModel.folder
    }

    ListView {
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        spacing: 10
        clip: true

        FolderListModel {
            id: folderModel
            showDotAndDotDot: false
            showOnlyReadable: true
            nameFilters: ["*.*"]
        }

        model: folderModel
        delegate: FileInfoDelegate { }
    }
}
