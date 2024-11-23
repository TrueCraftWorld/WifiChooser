import QtQuick 2.15


ListView {
    id: wifi_view
    width: 620
    // height:
    required model
    property var passwordPopUp: null

    signal networkChosen(id: int, passwd: string)



    function createPasswordPopUp(ssid_name) {
        if (passwordPopUp === null) {
            var component = Qt.createComponent("WiFiPassRequest.qml")
            if (component.status === Component.Ready){
                passwordPopUp = component.createObject( wifi_view, {"x":0, "y":0, "ssid_name": ssid_name})
                if (passwordPopUp) {
                    passwordPopUp.passwordAccepted.connect( tryConnection )
                    passwordPopUp.destroyMe.connect( deletePasswordPopUp )
                }
            }
        }
    }
    function tryConnection(passwd: string) {
        networkChosen(wifi_view.currentIndex, passwd)
        deletePasswordPopUp()
    }

    function deletePasswordPopUp() {
        if (passwordPopUp !== null) {
            passwordPopUp.destroy()
            passwordPopUp = null
        }
    }

    spacing: 5

    header: headerComponent
    clip: true

    delegate: Rectangle {
        id: wifi_deledgate

        required property string modelData
        property var view: ListView.view
        property bool isCurrent: ListView.isCurrentItem

        border.color: "darkblue"
        color: isCurrent ? "#157efb":"#53d769"
        radius: 10

        anchors.margins: 20
        height: 65
        width: ListView.view.width
        Text {
            id: ssid_string
            anchors.centerIn: parent
            text: parent.modelData
        }

        TapHandler {
            onTapped: {
                if (!isCurrent) {
                    // view.currentIndex = wifi_view.indexAt(wifi_deledgate.x+1,wifi_deledgate.y+1);
                    createPasswordPopUp(ssid_string.text)
                }
            }
        }
    }

    Component {
        id: headerComponent

        Rectangle {
            width: ListView.view.width
            height: 20
        }
    }
}
