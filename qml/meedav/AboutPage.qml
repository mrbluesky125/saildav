import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Page {
    id: aboutPage

    PageHeader {
        id: appTitleRect
        text: "MeeDav"
    }

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
            text: "Version 0.1"
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 64
            width: 64
            smooth: true
            //source: "images/SipgateSMS.png"
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            horizontalAlignment: Text.Center
            text: "Copyright (c) 2011 Timo Zimmermann<br />" +
                  "&lt;<a href='mailto:Timo Zimmermann meedav@timozimmermann.de'>meedav@timozimmermann.de</a>&gt;"
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            wrapMode: Text.WordWrap
            horizontalAlignment: Text.Center
            text: "MeeDav uses <br />
            Copyright (c) by :<br />
            <a href='https://'>https://</a><br />"
            onLinkActivated: Qt.openUrlExternally(link)
        }

    }
}
