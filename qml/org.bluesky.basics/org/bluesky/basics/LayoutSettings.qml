pragma Singleton

import QtQuick 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 1.2

Item {
    id: layoutSettings

    readonly property real normalizeFactor: 2.8             //

    property real pixelDensity: normalizeFactor
    property real scale: pixelDensity / normalizeFactor
    property int standardMargin: 8 * scale

    property LayoutTheme theme: LayoutTheme { }

    property QtObject color: QtObject {
        property color background: theme.background
        property color text: theme.text

        property color headerText: theme.text
        property color itemText: theme.text
        property color subItemText: theme.text
    }

    property LayoutFont font: LayoutFont { }
}
