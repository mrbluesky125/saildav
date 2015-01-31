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

                label: qsTr("About Saildav")
                text: qsTr("This app is a client to access online storage using HTTP/DAV protocol.\nIt is a Sailfish/QtQuick2 port of the Meego app MeeDAV.")
                separator: true
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Version")
                text: applicationVersion
                separator: true
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("Restrictions")
                text: qsTr("Die Zugangsinformationen werden im KLARTEXT in einer SQLite Datenbank gespeichert. Benutze die App nicht, falls du damit nicht einverstanden bist!")
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
                    text: "https://gitorious.org/saildav"
                    color: clickableUrl.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                onClicked: {
                    openInDefaultApp("https://gitorious.org/saildav");
                }
            }

            LabelWithCaption {
                anchors.left: parent.left
                anchors.margins: Theme.paddingLarge

                label: qsTr("References")
                text: qsTr("This project uses code and ideas of other projects, see README.md")
                separator: false
            }
        }
    }

    VerticalScrollDecorator { flickable: aboutPageFlickable }
}
