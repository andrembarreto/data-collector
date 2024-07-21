#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "mobilitydata.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MobilityData mobilityData;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mobilityData", &mobilityData);

    const QUrl url(u"qrc:/data-collector/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
