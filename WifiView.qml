import QtQuick 2.15
import com.melije.pulltorefresh 2.0
import StratifyLabs.UI 2.0

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

    clip: true


    delegate: SButton {
        id: wifi_deledgate
        property var view: ListView.view
        property bool isCurrent: ListView.isCurrentItem

        style: isconnected ? "btn-success lg":"btn-outline-secondary lg"
        anchors
        {
            topMargin: 15
            bottomMargin: 5
        }

        text: ssid
        onClicked: createPasswordPopUp(wifi_deledgate.text)
        width: ListView.view.width * .8
    }

    PullToRefreshHandler
    {
        onPullDownRelease:
        {
           updateMe() // Add your handling code here:
        }
    }
}
