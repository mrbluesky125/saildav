import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: pageStack.width
    height: sectionText.height

    Image {
        anchors.left: parent.left
        anchors.right: sectionText.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 12
        height: 2
        source: "image://theme/meegotouch-separator-background-horizontal"
    }

    Text {
        id: sectionText
        anchors.right: parent.right
        anchors.margins: 12
        text: section
        color: "darkgrey"
        font.pixelSize: 20
    }
}
