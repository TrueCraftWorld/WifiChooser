import QtQuick 2.15
import com.melije.pulltorefresh 2.0

ListView {
    id: wifi_view
    width: parent.width * .9
    required model
    property var passwordPopUp: null
    signal networkChosen(ssid: string, passwd: string)
    signal updateMe



    function createPasswordPopUp(ssid_name) {
        if (passwordPopUp === null) {
            var component = Qt.createComponent("WiFiPassRequest.qml")
            if (component.status === Component.Ready){
                passwordPopUp = component.createObject( wifi_view, {"x":0, "y":50, "ssid_name": ssid_name})
                if (passwordPopUp) {
                    passwordPopUp.passwordAccepted.connect( tryConnection )
                    passwordPopUp.destroyMe.connect( deletePasswordPopUp )
                }
            }
        }
    }
    function tryConnection(ssid: string, passwd: string) {
        networkChosen(ssid, passwd)
        deletePasswordPopUp()
    }

    function deletePasswordPopUp() {
        if (passwordPopUp !== null) {
            passwordPopUp.destroy()
            passwordPopUp = null
        }
    }

    spacing: 5

    // header: headerComponent
    clip: true


    delegate: Rectangle {
        id: wifi_deledgate

        // required property string ssid
        property var view: ListView.view
        property bool isCurrent: ListView.isCurrentItem


        border.color: "#005c9f"
        // color:  "#d8d8d8"
        color: isconnected ? "#005c9f":"#d8d8d8"
        radius: 10

        anchors.margins: 20
        height: 65
        width: ListView.view.width
        Text {
            id: ssid_string
            anchors.centerIn: parent
            text: ssid
            // color:  "black"
            color: isconnected ? "white":"black"
        }

        TapHandler {
            onTapped: {
                createPasswordPopUp(ssid_string.text)
            }
        }
    }

    PullToRefreshHandler
    {
        onPullDownRelease:
        {
           updateMe() // Add your handling code here:
        }
    }
}
