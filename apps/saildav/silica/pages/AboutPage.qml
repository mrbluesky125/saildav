import QtQuick 2.1
import Sailfish.Silica 1.0

import components 1.0

Page {
    id: aboutPage

    SilicaFlickable {
        id: aboutPageFlickable
        anchors.fill: parent
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height

            PageHeader {
                title: qsTr("About Saildav")
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("About SailDAV")
                text: qsTr("This app is a client to access online storage using HTTP/DAV protocol. It is a Sailfish port of the Meego app MeeDAV.")
                separator: true
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Version")
                text: getVersion()
                separator: true
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Author")
                text: "Timo Zimmermann"
                separator: true
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Contributors")
                text: "Corentin Chary"
                separator: true
            }

            BackgroundItem {
                id: clickableUrl
                contentHeight: labelUrl.height
                height: contentHeight
                width: aboutPageFlickable.width
                anchors.left: parent.left

                LabelWithCaption {
                    id: labelUrl
                    anchors.left: parent.left
                    anchors.margins: Theme.paddingLarge

                    label: qsTr("Source code")
                    text: "http://git.timozimmermann.de/timo/saildav"
                    color: clickableUrl.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                onClicked: {
                    openInDefaultApp("http://git.timozimmermann.de/timo/saildav");
                }
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("References")
                text: qsTr("This project uses code and ideas of other projects, see README.md.")
                separator: true
            }
        }
    }

    VerticalScrollDecorator { flickable: aboutPageFlickable }
}
