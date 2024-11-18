import QtQuick 2.15

ListView {
    id: wifi_view
    width: 620
    height: 500
    required model


    spacing: 5

    header: headerComponent
    clip: true

    delegate: Rectangle {
        id: wifi_deledgate

        required property string modelData
        property var view: ListView.view
        property var isCurrent: ListView.isCurrentItem

        border.color: "darkblue"
        color: isCurrent ? "#157efb":"#53d769"


        anchors.margins: 20
        height: 35
        width: ListView.view.width
        Text {
            text: parent.modelData
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                view.currentIndex = model.index;
            }
        }
    }

    Component {
        id: headerComponent

        Rectangle {
            width: ListView.view.width
            height: 20
        }
    }
}
