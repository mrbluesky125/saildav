import QtQuick 2.2
import "." //Singleton import

Item {
    width: parent.width
    height: (view.count+2) * 90
    visible: view.count !== 0

    function showNotification(caption, infoText) {
        var notification = { "caption": caption, "infoText": infoText }
        tooltipModel.append( notification )
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
            NumberAnimation {
                easing.overshoot: 1.9
                easing.type: Easing.OutBack
                properties: "x,y"
                duration: 200
                alwaysRunToEnd: true
            }
        }
        add: Transition {
            NumberAnimation {
                property: "scale"
                easing.overshoot: 1.9
                easing.type: Easing.OutBack
                from: 0.0
                to: 1.0
                duration: 200
                alwaysRunToEnd: true
            }
        }
        remove: Transition {
            NumberAnimation {
                property: "opacity"
                easing.type: Easing.OutBack
                from: 1.0
                to: 0.0
                duration: 200
                alwaysRunToEnd: true
            }
        }

        delegate: Notification {
            id: delegateItem
            text: caption
            description: infoText
            onClosed: tooltipModel.remove(index) //opacity = 0           //fade out on closing
            Component.onCompleted: show()

            //Behavior on opacity { NumberAnimation {} }

            //onVisibleChanged: if(opacity === 0.0) tooltipModel.remove(index);
        }
    }
}
