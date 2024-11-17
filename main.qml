import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Extras 1.4

import BackEnd 1.0


ApplicationWindow {
    id: root
    width: 1280
    height: 800
    visible: true
    color: "Black"

    NetworkSearch {
        id: wifi_handle
    }

    Rectangle {
        id: butt
        width: 400
        height: 200
        anchors.centerIn: parent
        color: "blue"

        Button {
            Rectangle {
                anchors.fill: parent
                color: "gold"
            }

            anchors.fill: parent
            onClicked: {
                console.log("bla");
                wifi_handle.updateWiFiInfo();
                // butt.color = butt.color == "blue" ? "white" : "blue"

            }

        }
    }
}
