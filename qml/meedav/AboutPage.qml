import QtQuick 1.1
import com.nokia.meego 1.0
import "components"

Page {
    id: aboutPage

    PageHeader {
        id: appTitleRect
        text: "MeeDav"
    }

<<<<<<< HEAD
    tools: topLevelTools

=======
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
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
<<<<<<< HEAD
            source: "images/meedav.svg"
=======
            //source: "images/SipgateSMS.png"
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
<<<<<<< HEAD
            font.pixelSize: 20
=======

>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
            horizontalAlignment: Text.Center
            text: "Copyright (c) 2011 Timo Zimmermann<br />" +
                  "&lt;<a href='mailto:Timo Zimmermann meedav@timozimmermann.de'>meedav@timozimmermann.de</a>&gt;"
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
<<<<<<< HEAD
            font.pixelSize: 20
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.Center
            text: "MeeDav is based on QWebdav by Corentin Chary<br />
            Copyright (c) by Corentin Chary<br />
            <a href='http://git.iksaif.net/?p=qwebdav.git;a=summary'>http://git.iksaif.net/?p=qwebdav.git;a=summary</a><br />"
=======

            wrapMode: Text.WordWrap
            horizontalAlignment: Text.Center
            text: "MeeDav uses <br />
            Copyright (c) by :<br />
            <a href='https://'>https://</a><br />"
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
            onLinkActivated: Qt.openUrlExternally(link)
        }

    }
}

