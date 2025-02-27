import QtQuick 2.15

Rectangle {
    id: button
    property string text
    signal tapped
    width: 300
    height: 80
    color: "darkslateblue"
    border.color: "burlywood"
    Text {
        id: butText
        anchors.fill: parent
        color: "burlywood"
        text: button.text
    }
    MouseArea {
        anchors.fill: parent
        onClicked: button.tapped()
    }
}
