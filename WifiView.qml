import QtQuick 2.15
import QtQuick.Layouts 1.15
import com.melije.pulltorefresh 2.0
import StratifyLabs.UI 2.0

ListView {
    id: wifi_view
    width: parent.width

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


    layoutDirection: Qt.LeftToRight
    verticalLayoutDirection: ListView.TopToBottom
    displayMarginBeginning: 15
    displayMarginEnd: 15
    spacing: 5

    clip: true

    header: Item { width: parent.width; height: 15 }
    footer: Item { width: parent.width; height: 15 }
    delegate: SButton {
        id: wifi_deledgate
        property var view: ListView.view
        property bool isCurrent: ListView.isCurrentItem

        style: isconnected ? "btn-success lg":"btn-outline-secondary lg"
        anchors
        {
            horizontalCenter: parent.horizontalCenter
            topMargin: 15
            bottomMargin: 5
        }

        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
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
