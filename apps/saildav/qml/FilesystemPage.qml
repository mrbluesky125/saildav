import QtQuick 2.0

import Qt.labs.folderlistmodel 1.0

import org.bluesky.basics 1.0
import org.bluesky.models 1.0
import qwebdav 1.0

import "delegates"

Page {
    id: root

    property WebdavModel model

//    tools: ToolBarLayout {
//        ToolIcon {
//            platformIconId: "toolbar-back"
//            onClicked: folderModel.folder = folderModel.parentFolder
//        }
//        ToolIcon {
//            platformIconId: "toolbar-close"
//            onClicked: appWindow.pageStack.pop()
//        }
//    }

    Header {
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

        function upload(localpath) {
            model.upload(model.folder, localpath);
            stackView.pop();
        }
    }
}
