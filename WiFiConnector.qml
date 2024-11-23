import QtQuick 2.15
import QtQuick.Controls 2.15

import BackEnd 1.0

Item {
    id: wifiRoot

    NetworkSearch {
        id: wifi_handle
        onAvailableWiFiNetsChanged: {
            ssid_selector.model = wifi_handle.availableWiFiNets
        }
    }

    Button {
        id: butt
        width: 1280
        height: 80
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        // anchors.fill: parent
        onClicked: {
            console.log("bla");
            wifi_handle.updateWiFiInfo();
        }
        Text {
            anchors.fill: parent
            color: "black"
            text: "Обновить список"
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

    Rectangle {
        id: ipContainer
        width: 1280-800
        height: 720

        color: "darkorchid"
        opacity: 0.75
        border {
            width: 2
            color: "black"
        }

        anchors {
            top: butt.bottom
            left: viewContainer.right
        }
        Text {
            id: ip_text

            width: parent.width
            anchors {
                top: parent.top
                left: parent.left
                topMargin: 15

            }
            text: "IP:"
        }
    }



    Connections {
        target: ssid_selector
        function onNetworkChosen(idx: int, passwd: string) {
            wifi_handle.tryConnect(idx, passwd)
            ip_text.text = "IP: " + wifi_handle.getWifiIP()
        }
    }
}
