import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    id: page
    width: parent.width
    height: parent.height

    property int stackIndex: Stack.index
    property StackView stackView: Stack.view
    property int stackStatus: Stack.status


}
