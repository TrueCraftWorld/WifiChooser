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
        onWifiStateChanged: {
            if (wifi_handle.wifiState) {
                ssid_selector.visible = true
                ssid_selector.model = wifi_handle.availableWiFiNets
            } else {
                ssid_selector.visible = false
            }
        }
    }

    Button {
        id: butt
        width: 640
        height: 80
        anchors {
            left: parent.left
            top: parent.top
        }
        onClicked: {
            wifi_handle.updateWiFiInfo();
        }
        Text {
            anchors.fill: butt
            color: "black"
            text: "Обновить список"
        }
    }
    Button {

        id: butt2
        width: 640
        height: 80
        anchors {
            left: butt.right
            top: parent.top
        }
        onClicked: {
            wifi_handle.wifiState = !wifi_handle.wifiState
        }
        Text {
            anchors.fill: butt2
            color: "black"
            text: wifi_handle.wifiState ? "WiFi запущен" : "WiFi выключен"
        }
    }

    Rectangle {
        id: viewContainer
        width: 650
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
            height: viewContainer.height
            model: wifi_handle.availableWiFiNets
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Rectangle {
        id: ipContainer
        width: 1280-650
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
                leftMargin: 10

            }
            text: "IP: " + wifi_handle.currentIp
        }
        Text {
            id: nmcli_respond
            width: parent.width
            anchors {
                top: ip_text.bottom
                left: parent.left
                topMargin: 15
                leftMargin: 10
            }
        }
    }

    Connections {
        target: ssid_selector
        function onNetworkChosen(idx: int, passwd: string) {
            console.log("pass try")
            wifi_handle.tryConnect(idx, passwd)
        }

    }
}
