import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: control
    // required property alias
    // text: qsTr("Button")

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        // color: control.down ? "#17a81a" : "#21be2b"
        color: enabled ? "#005c9f" : "#484848"
        //0 92 159
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        // border.color: control.down ? "#17a81a" : "#21be2b"
        border.color:  "#005c9f"
        border.width: 1
        radius: 8
    }
}
