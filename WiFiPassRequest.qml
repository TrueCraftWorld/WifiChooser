import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: pass_req_root
    width: 600
    height: 300
    radius: 10
    opacity: 1
    property string ssid_name
    signal passwordAccepted(ssid: string, password: string)
    signal destroyMe()
    anchors {
        top: parent.top
        topMargin: 50
        horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: pass_req_title
        width: parent.width
        height: 50
        anchors {
            left: parent.left
            top: parent.top
            topMargin: 15
            horizontalCenter: parent.horizontalCenter
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr("ВВЕДИТЕ ПАРОЛЬ")
    }

    Rectangle {
        id: pass_req_ssid
        width: parent.width * .8
        height: 45
        radius: 8
        anchors {
            top: pass_req_title.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        Text {
            anchors.fill: parent
            text: "SSID: " + ssid_name
            anchors.leftMargin: 15
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: pass_req_pswd
        width: parent.width * .8
        height: 45
        radius: 8
        anchors {
            top: pass_req_ssid.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        TextInput {
            id: passwd_text
            anchors.fill: parent
            anchors.leftMargin: 15
            verticalAlignment: Text.AlignVCenter
            echoMode: TextInput.Password
        }
    }
    StyledButton {
        id: pass_req_accept_button
        width: parent.width * .35
        height: 45
        anchors {
            top: pass_req_pswd.bottom
            topMargin: 20
            left: parent.left
            leftMargin: parent.width * .1
        }
        Text {
            anchors.centerIn: parent
            text: qsTr("Подключить")
        }
        onClicked: {
            passwordAccepted(ssid_name, passwd_text.text)
        }
    }
    StyledButton {
        id: pass_req_cancel_button
        width: parent.width * .35
        height: 45
        anchors {
            top: pass_req_pswd.bottom
            topMargin: 20
            right: parent.right
            rightMargin: parent.width * .1
        }
        Text {
            anchors.centerIn: parent
            text: qsTr("Отмена")
        }
        onClicked: {
            destroyMe()
        }
    }
}
