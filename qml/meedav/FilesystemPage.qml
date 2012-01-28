import QtQuick 1.1
import com.nokia.meego 1.0
import Qt.labs.folderlistmodel 1.0
import "components"
import "delegates"

Page {
    tools: commonTools

    PageHeader {
        id: appTitleRect
        text: ""
        busy: false
    }

    ListView {
        width: 200; height: 400

        FolderListModel {
            id: folderModel
            nameFilters: ["*.qml"]
        }

        Component {
            id: fileDelegate
            Text { text: fileName }
        }

        model: folderModel
        delegate: fileDelegate
    }
}
