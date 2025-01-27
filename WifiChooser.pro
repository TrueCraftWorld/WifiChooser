include(wifichooser.pri)

SOURCES += \
        main.cpp \

RESOURCES += qml.qrc \

QT_QPA_ENABLE_TERMINAL_KEYBOARD = 0


# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

target.path = /usr/share/qtpr
INSTALLS += target


