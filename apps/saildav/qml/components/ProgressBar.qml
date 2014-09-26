import QtQuick 1.1

Rectangle {
    id: root

    property bool running: false
    property double itemProgress: 0.0

    width: parent.width*itemProgress

    color: "#32c81e"
    opacity: running ? 0.5 : 0.0

    Behavior on width { NumberAnimation {} }
    Behavior on opacity { NumberAnimation {} }
}
