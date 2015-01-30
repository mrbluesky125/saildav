import QtQuick 2.0
import Sailfish.Silica 1.0

import pulse 1.0

Page {
    id: page

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            busy: pulseDetector.state === PulseDetector.StateRunning
            MenuItem {
                text: pulseDetector.state === PulseDetector.StateRunning ? qsTr("Stoppe Messung") : qsTr("Starte Messung")
                onClicked: {
                    if(pulseDetector.state === PulseDetector.StateRunning)
                        pulseDetector.stopDetection()
                    else
                        pulseDetector.startDetection()
                }
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Messung")
            }
            ProgressCircle {
                id: progressCircle
                anchors.horizontalCenter: parent.horizontalCenter
                width: 300
                height: 300
                value: 30000./pulseDetector.interval

                Label {
                    anchors.centerIn: parent
                    text: qsTr("HR:") + pulseDetector.heartRate
                    color: Theme.secondaryHighlightColor
                    font.pixelSize: Theme.fontSizeExtraLarge
                }
            }

        }
    }
}


