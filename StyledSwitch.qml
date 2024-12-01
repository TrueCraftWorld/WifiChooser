import QtQuick 2.15
import QtQuick.Controls 2.15

Switch {
    id: switchRoot


    text: qsTr("Wi-Fi")

    indicator: Rectangle {
        implicitWidth: 48
        implicitHeight: 26
        x: switchRoot.leftPadding
        y: parent.height / 2 - height / 2
        radius: 13
        // color: switchRoot.checked ? "#17a81a" : "#ffffff"
        color:  enabled ? "#005c9f" : "#999999"
        // border.color: switchRoot.checked ? "#17a81a" : "#cccccc"
        border.color:  enabled ? "#005c9f" : "#999999"

        Rectangle {
            x: switchRoot.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: switchRoot.down ? "#cccccc" : "#ffffff"
            border.color: switchRoot.checked ? ("#005c9f") : "#999999"
        }
    }

    contentItem: Text {
        text: switchRoot.text
        font: switchRoot.font
        opacity: enabled ? 1.0 : 0.3
        // color: switchRoot.down ? "#17a81a" : "#21be2b"
        color: "white"
        // font.bold: true
        verticalAlignment: Text.AlignVCenter
        leftPadding: switchRoot.indicator.width + switchRoot.spacing
    }


    anchors {
        top: parent.top
        left: parent.left
        leftMargin: 25
        topMargin: 25
    }
}
