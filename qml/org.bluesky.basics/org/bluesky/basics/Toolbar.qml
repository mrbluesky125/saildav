import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.0

import "."

ToolBar {
    id: toolbar
    width: parent.width
    height: 60 * LayoutSettings.scale

    property list<Action> actionList

    Row {
        id: row
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        Repeater {
            id: repeater
            model: actionList

            ToolButton {
               height: toolbar.height
               width: toolbar.width / row.children.length
               action: actionList[index]
            }
        }
    }
}

