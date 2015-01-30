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
                title: qsTr("Über Saildav")
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Über Saildav")
                text: qsTr("This app is a client to access online storage using HTTP/DAV protocol.\nIt is a Sailfish port of the Meego app MeeDAV.")
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

                label: qsTr("Einschränkungen")
                text: qsTr("Die Zugangsinformationen werden im KLARTEXT in einer SQLite Datenbank gespeichert. Benutze die App nicht falls du damit nicht einverstanden bist!")
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Author")
                text: "Timo Zimmermann"
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Contributors")
                text: "Corentin Chary"
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
                separator: false
            }
        }
    }

    VerticalScrollDecorator { flickable: aboutPageFlickable }
}
