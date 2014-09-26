import QtQuick 2.2

Item {
    width: parent.width
    height: (view.count+2) * 90

    function showNotification(caption, infoText) {
        tooltipModel.insert(0, { "caption": caption, "infoText": infoText } )
    }

    ListModel {
        id: tooltipModel
    }

    ListView {
        id: view
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.fill: parent
        anchors.margins: 8
        model: tooltipModel
        clip: false
        spacing: 8

        displaced: Transition {
            NumberAnimation { easing.overshoot: 1.9; easing.type: Easing.OutBack; properties: "x,y"; duration: 200 }
        }
        add: Transition {
            NumberAnimation { property: "scale"; easing.overshoot: 1.9; easing.type: Easing.OutBack; from: 0.0; to: 1.0; duration: 200 }
        }

        delegate: Tooltip {
            id: delegateItem
            text: caption + " #" + index
            description: infoText
            onClosed: opacity = 0           //fade out on closing
            Component.onCompleted: show()

            onVisibleChanged: if(!visible) tooltipModel.remove(index);
        }
    }
}
