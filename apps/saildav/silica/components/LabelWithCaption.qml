import QtQuick 2.1
import Sailfish.Silica 1.0

Column {
    property alias label: label.text
    property alias text: text.text
    property alias font: text.font
    property alias separator: separator.visible
    property alias color: text.color

    spacing: Theme.paddingMedium

    anchors.right: parent.right
    anchors.left: parent.left

    Label {
        id: label
        anchors.left: parent.left
        width: parent.width
        color: Theme.highlightColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }
    Label {
        id: text
        anchors.left: parent.left
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
        wrapMode: Text.Wrap
        width: parent.width - (2 * Theme.paddingLarge)
    }
    Separator {
        id: separator
        width:parent.width;
        color: Theme.highlightColor
    }
}
