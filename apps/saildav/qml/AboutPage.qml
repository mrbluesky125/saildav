import QtQuick 2.0
import QtQuick.Controls 1.1

import "components"

Page {
    id: aboutPage

    PageHeader {
        id: appTitleRect
        text: "MeeDav"
        iconEnabled: true
        iconSource: "images/meedav.svg"
    }

    tools: topLevelTools

    Column {
        id: column
        anchors.top: appTitleRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 12
        spacing: 25

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            horizontalAlignment: Text.Center
            text: "Version 0.1.3" //+ version
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 64
            width: 64
            smooth: true

            source: "images/meedav.svg"

        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 20

            horizontalAlignment: Text.Center
            text: "Copyright (c) 2011 Timo Zimmermann<br />" +
                  "&lt;<a href='mailto:Timo Zimmermann meedav@timozimmermann.de'>meedav@timozimmermann.de</a>&gt;"
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            font.pixelSize: 20
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.Center
            text: "MeeDav is based on QWebdav by Corentin Chary<br />
            Copyright (c) by Corentin Chary<br />
            <a href='http://git.iksaif.net/?p=qwebdav.git;a=summary'>http://git.iksaif.net/?p=qwebdav.git;a=summary</a><br />"
            onLinkActivated: Qt.openUrlExternally(link)
        }

    }
}

