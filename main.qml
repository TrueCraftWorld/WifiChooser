import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Extras 1.4
// import QtQuick.Controls 1.4




ApplicationWindow {
    id: root
    width: 1280
    height: 800
    visible: true
    color: "Black"

    WiFiConnector {
        anchors.fill: parent
    }

}
