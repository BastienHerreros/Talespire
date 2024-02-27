#include "controllers/ReaderCtrl.hpp"
#include "models/AssetModel.hpp"
#include "models/LayoutModel.hpp"
#include "widgets/ImageDisplayWidget.hpp"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    // Create the QApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QApplication::setApplicationName("ReaderApp");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Juflasse Inc");

    // Init the qml engine
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc://.");
    Q_INIT_RESOURCE(qml);

    // Register types
    qmlRegisterType<app::ui::ImageDisplayWidget>("cpp.widgets", 1.0, 0, "ImageDisplayWidget");
    qmlRegisterType<app::models::AssetModel>("cpp.models", 1.0, 0, "AssetModel");
    qmlRegisterType<app::models::LayoutModel>("cpp.models", 1.0, 0, "LayoutModel");
    qmlRegisterType<app::controllers::ReaderCtrl>("ReaderCtrl", 1.0, 0, "ReaderCtrl");

    // load main qml file
    engine.load(QUrl(QLatin1String("qrc:/app/mainApplication.qml")));

    const bool errorCode = app.exec();

    return errorCode;
}