import QtQuick 2.15
import QtQuick.Controls 2.15

import BackEnd 1.0
import StratifyLabs.UI 2.0

Item {
    id: wifiRoot
    signal returnButtonPressed();

    NetworkSearch {
        id: wifi_handle
        // onAvailableWiFiNetsChanged: {}
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

    SLabel{
      style: "btn-naked"
      id: topStatus

      height: 50
      anchors{
          left: parent.left
          right: parent.right
          top: parent.top
      }
      text: qsTr("Настройки сети")
    }

    SContainer {
        id: viewContainer
        width: parent.width * .6

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
        SLabel {
            id: ssid_selector_title
            style: "label-info lg";

            height: 50
            anchors {
                left:  parent.left
                right: parent.right
                top: parent.top
            }
            text :qsTr("Доступные Wi-Fi сети")
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

    SContainer {
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
        SLabel {
            id: common_title
            style: "label-info lg";

            height: 50
            anchors {
                left:  parent.left
                right: parent.right
                top: parent.top
            }
            text :qsTr("Общие")
        }
        StyledSwitch {
            id:wifiSwitch

            anchors {
                top: common_title.bottom
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

        SLabel {
            id: ip_title
            anchors {
                top: vpnSwitch.bottom
                left: parent.left
                topMargin: 40
                leftMargin: 10
                rightMargin: 10
            }
            style: "btn-naked"
            text:qsTr("Текущий IP-адрес устройства:")
        }

        SLabel {
            id: ip_text

            anchors {
                top: ip_title.bottom
                left: parent.left
                topMargin: 15
                leftMargin: 10
                rightMargin: 10
            }
            style: "label-info"
            text: wifi_handle.currentIp
        }

        StyledBusyIndicator {
            id: wifi_busy
            running: false
            anchors.fill: parent
        }

    }

    SButton {
        id: returnButton
        style: "btn-secondary"
        text: qsTr("Назад")
        anchors {
            left:parent.left
            bottom: parent.bottom
            margins: 10
        }
        onClicked: updateRequester.returnButtonPressed()
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
