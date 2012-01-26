import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "lightsteelblue";
        }
        GradientStop {
            position: 1.00;
            color: Qt.darker("lightsteelblue", 1.2);
        }
    }
    border.width: 0

}
