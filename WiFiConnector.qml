import QtQuick 2.15
import QtQuick.Controls 2.15

import BackEnd 1.0

Item {
    id: wifiRoot
    // property var theModel: null
    NetworkSearch {
        id: wifi_handle
        // property wifiModel theModel: wifi_handle.wifiModel
        onAvailableWiFiNetsChanged: {
            // ssid_selector.model = wifi_handle.availableWiFiNets
            // ssid_selector.currentIndex = wifi_handle.activeSsidIdx
        }
        onWifiStateChanged: {
            if (wifi_handle.wifiState) {
                ssid_selector.visible = true
                // ssid_selector.model = wifi_handle.wifiList
            } else {
                ssid_selector.visible = false
            }
            wifi_busy.running = false;
        }
        onCommandFinished: {
            wifi_busy.running = false;
        }
    }
    // Component.onCompleted: {
    //     theModel = wifi_handle.wifiModel
    //     ssid_selector.model = theModel
    //     // theModel.lo
    // }

    UpdateClient {
        id: update_handle
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
        Button {
            id: update_request

            width: 150
            height: 50
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: ip_text.bottom
                topMargin: 45
            }
            onClicked: {
                // console.log("update_press")
                update_handle.requestUpdate()

            }
        }

        StyledBusyIndicator {
            id: wifi_busy
            running: false
            anchors.fill: parent
        }

    }

    Connections {
        target: ssid_selector
        function onNetworkChosen(ssid: string, passwd: string) {
            wifi_handle.suspendNetSearch()
            wifi_handle.tryConnect(ssid, passwd)
            wifi_busy.running = true;
            // wifi_handle.resumeNetSearch()
        }
        function onUpdateMe() {
            wifi_handle.updateWiFiInfo()
        }
    }

}
