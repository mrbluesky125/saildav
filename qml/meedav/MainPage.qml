import QtQuick 1.1
import com.nokia.meego 1.0
<<<<<<< HEAD
import "components"
import "delegates"
=======
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a

Page {
    tools: commonTools

<<<<<<< HEAD
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

=======
    Label {
        id: label
        anchors.centerIn: parent
        text: qsTr("Hello world!")
        visible: false
    }

    Button{
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: label.bottom
            topMargin: 10
        }
        text: qsTr("Click here!")
        onClicked: label.visible = true
    }
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}
