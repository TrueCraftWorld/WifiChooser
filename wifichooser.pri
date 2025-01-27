include($$PWD/3rdParty/PullToRefreshHandler.pri)

QT += quick core network qml concurrent
CONFIG += c++17 link_pkgconfig disable-desktop
 

SOURCES += \
        $$PWD/NetworkDiscover.cpp \
        $$PWD/updateclient.cpp \
        $$PWD/wifilistmodel.cpp

RESOURCES += $$PWD/wifimodule.qrc

HEADERS += \
    $$PWD/NetworkDiscover.h \
    $$PWD/package.h \
    $$PWD/protocolcommand.h \
    $$PWD/updateConfig.h \
    $$PWD/updateclient.h \
    $$PWD/wifilistmodel.h

