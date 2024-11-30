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

    Rectangle {
        id: topStatus

        height: 50
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
        }
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            color: "black"
            text: qsTr("Настройки сети")
        }
    }

    Rectangle {
        id: viewContainer
        width: parent.width * .6
        // height: parent.height

        color: "darkslategray"
        opacity: 0.75
        border {
            width: 1
            color: "black"
        }

        anchors {
            top: topStatus.bottom
            left: ipContainer.right
            bottom: parent.bottom
        }
        Rectangle{
            id: ssid_selector_title
            color: "darkslategray"
            width: parent.width
            height: 50
            anchors {
                top: parent.top
            }
            Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: qsTr("Доступные Wi-Fi сети")
            }
        }

        WifiView {
            id: ssid_selector
            height: viewContainer.height
            model: wifi_handle.availableWiFiNets
            anchors {
                top: ssid_selector_title.bottom
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Rectangle {
        id: ipContainer
        width: parent.width * .4
        // height: parent.height

        color: "darkslategray"
        opacity: 0.75
        border {
            width: 1
            color: "black"
        }

        anchors {
            top: topStatus.bottom
            left: parent.left
            bottom: parent.bottom
        }
        StyledSwitch {
            id:wifiSwitch

            anchors {
                top: parent.top
                left: parent.left
                leftMargin: 25
                topMargin: 25
            }

            text: qsTr("Wi-Fi")
            checked:  wifi_handle.wifiState

            onToggled: {
                wifi_handle.wifiState = wifiSwitch.checked
            }

        }
        StyledSwitch {
            id: vpnSwitch
            anchors {
                top: wifiSwitch.bottom
                left: parent.left
                leftMargin: 25
                topMargin: 15
            }
            text: qsTr("VPN FOTEK")

            enabled: false
        }

        Text {
            id: ip_title

            width: parent.width
            anchors {
                top: vpnSwitch.bottom
                left: parent.left
                topMargin: 40
                leftMargin: 10

            }
            color: "white"
            text:qsTr("Текущий IP-адрес устройства:")
        }

        Text {
            id: ip_text

            width: parent.width
            anchors {
                top: ip_title.bottom
                left: parent.left
                topMargin: 15
                leftMargin: 10

            }
            color: "white"
            text: wifi_handle.currentIp
        }

    }

    Connections {
        target: ssid_selector
        function onNetworkChosen(idx: int, passwd: string) {
            wifi_handle.tryConnect(idx, passwd)
        }
        function onUpdateMe() {
            wifi_handle.updateWiFiInfo()
        }
    }

}
