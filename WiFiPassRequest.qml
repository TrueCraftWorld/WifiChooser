import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: pass_req_root
    width: 600
    height: 400
    radius: 10

    property string ssid_name
    signal passwordAccepted(password: string)
    signal destroyMe()

    Text {
        id: pass_req_title
        width: parent.width
        height: 50
        anchors {
            left: parent.left
            top: parent.top
        }
        text: "ВВЕДИТЕ ПАРОЛЬ"
    }

    Rectangle {
        id: pass_req_ssid
        width: parent.width * .8
        height: 100
        radius: 8
        anchors {
            top: pass_req_title.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        Text {
            anchors.fill: parent
            text: "SSID: " + ssid_name
        }
    }
    Rectangle {
        id: pass_req_pswd
        width: parent.width * .8
        height: 100
        radius: 8
        anchors {
            top: pass_req_ssid.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        TextInput {
            id: passwd_text
            anchors.fill: parent
        }
    }
    Button {
        id: pass_req_accept_button
        width: parent.width * .8
        height: 40
        anchors {
            top: pass_req_pswd.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        Text {
            anchors.fill: parent
            text: "OK"
        }
        onClicked: {
            passwordAccepted(passwd_text.text)
        }
    }
    Button {
        id: pass_req_cancel_button
        width: parent.width * .8
        height: 40
        anchors {
            top: pass_req_accept_button.bottom
            topMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        Text {
            anchors.fill: parent
            text: "Cancel"
        }
        onClicked: {
            destroyMe()
        }
    }
}
