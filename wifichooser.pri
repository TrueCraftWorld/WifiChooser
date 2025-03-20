include($$PWD/3rdParty/PullToRefreshHandler.pri)

QT += quick core network qml concurrent
CONFIG += c++17 link_pkgconfig disable-desktop
 

SOURCES += \
        $$PWD/NetworkDiscover.cpp \
        $$PWD/wifilistmodel.cpp

RESOURCES += $$PWD/wifimodule.qrc

HEADERS += \
    $$PWD/NetworkDiscover.h \
    $$PWD/wifilistmodel.h

DISTFILES += \
    $$PWD/ServiceButton.qml \
    $$PWD/StyledBusyIndicator.qml \
    $$PWD/StyledButton.qml \
    $$PWD/StyledSwitch.qml \
    $$PWD/WiFiConnector.qml \
    $$PWD/WiFiPassRequest.qml \
    $$PWD/WifiView.qml

