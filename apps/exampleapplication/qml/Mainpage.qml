import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import org.emblab.example 1.0

ApplicationWindow {
    width: 1024
    height: 768

    LayoutSettings {
        id: appLayoutSettings
    }

    StackView {
        id: mainStackView
        anchors.fill: parent

        initialItem: Qt.resolvedUrl("ExampleStackPage.qml")
    }

    Component.onCompleted: show()
}
