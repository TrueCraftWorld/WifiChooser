#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "NetworkDiscover.h"
#include "updateclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
///Добавляем модуль клавиатуры
    qputenv("QT_IM_MODULE", QByteArray("cutekeyboard"));

    QGuiApplication app(argc, argv);

    NetworkControl::registerNetworkControl();
    UpdateClient::registerUpdateClient();

    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
