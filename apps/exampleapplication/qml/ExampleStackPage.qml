import QtQuick 2.2
import QtQuick.Controls 1.2

import org.emblab.example 1.0

Page {
    id: root

    Label {
        anchors.centerIn: parent
        text: qsTr("Hallo Welt Page! Stack index: ") + stackIndex
        font.pointSize: appLayoutSettings.font.standardPointSize
    }

    Header {
        text: qsTr("Title text")
        backButton.opacity: stackIndex > 0
        onBackClicked: stackView.pop()
    }

}
