import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Extras 1.4
// import QtQuick.Controls 1.4

import BackEnd 1.0


ApplicationWindow {
    id: root
    width: 1280
    height: 800
    visible: true
    color: "Black"

    NetworkSearch {
        id: wifi_handle
        onAvailableWiFiNetsChanged: {
            ssid_selector.model = wifi_handle.availableWiFiNets
        }
    }
    Rectangle {
        id: butt
        width: 1280
        height: 80
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        Button {
            Rectangle {
                anchors.fill: parent
                color: "gold"
                Text {
                    anchors.fill: parent
                    color: "black"
                    text: "Обновить список"
                }
            }
            anchors.fill: parent
            onClicked: {
                console.log("bla");
                wifi_handle.updateWiFiInfo();
            }
        }
    }
    Rectangle {
        id: viewContainer
        width: 800
        height: 720

        color: "darkorchid"
        opacity: 0.75
        border {
            width: 2
            color: "black"
        }

        anchors {
            top: butt.bottom
            left: parent.left
        }
        WifiView {
            id: ssid_selector
            // currentIndex: 2
            height: viewContainer.height
            model: wifi_handle.availableWiFiNets
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
    Connections {
        target: ssid_selector
        function onNetworkChosen(idx: int, passwd: string) {
            wifi_handle.tryConnect(idx, passwd)
        }
    }
}
