import QtQuick 2.15
import QtQuick.Controls 2.15

import BackEnd 1.0

Item {
    id: wifiRoot
    signal returnButtonPressed();
    NetworkSearch {
        id: wifi_handle
        onAvailableWiFiNetsChanged: {
        }
        onWifiStateChanged: {
            if (wifi_handle.wifiState) {
                ssid_selector.visible = true
            } else {
                ssid_selector.visible = false
            }
            wifi_busy.running = false;
        }
        onCommandFinished: {
            wifi_busy.running = false;
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
            verticalAlignment: Text.AlignVCenter
            // font.bold: true
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
                verticalAlignment: Text.AlignVCenter
                // font.bold: true
                color: "white"
                text: qsTr("Доступные Wi-Fi сети")
            }
        }

        WifiView {
            id: ssid_selector
            height: viewContainer.height
            model: wifi_handle.wifiModel
            enabled: !wifi_busy.running
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

        StyledBusyIndicator {
            id: wifi_busy
            running: false
            anchors.fill: parent
        }

    }
    Rectangle {
        id: returnButton
        anchors {
            left:parent.left
            bottom: parent.bottom
        }
        MouseArea {
            anchors.fill: parent
            onClicked: wifiRoot.returnButtonPressed()
        }
        width: 55
        height: 55
        color: "darkblue"
    }
    Connections {
        target: ssid_selector
        function onNetworkChosen(ssid: string, passwd: string) {
            wifi_handle.suspendNetSearch()
            wifi_handle.tryConnect(ssid, passwd)
            wifi_busy.running = true;
        }
        function onUpdateMe() {
            wifi_handle.updateWiFiInfo()
        }
    }

}
