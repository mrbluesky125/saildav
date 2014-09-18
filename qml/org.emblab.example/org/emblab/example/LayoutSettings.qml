import QtQuick 2.2
import QtQuick.Window 2.0

QtObject {
    id: layoutSettings

    property real scaleFactor: 2.5
    readonly property real scale: Screen.pixelDensity / scaleFactor
    readonly property int intScale: Math.max(1, scale)

    property QtObject color: QtObject {
        property string headerTextColor: "black"
        property string standardTextColor: "black"
        //add application colors here
    }

    property QtObject font: QtObject {
        property int headerPointSize: 26
        property int standardPointSize: 16
        //add application text sizes here
    }

}
