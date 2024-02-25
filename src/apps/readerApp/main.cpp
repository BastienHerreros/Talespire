#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char* argv[])
{
    // Create the QApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    // set the style
    QQuickStyle::setStyle("Material");

    QApplication::setApplicationName("ReaderApp");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Juflasse Inc");

    // Init the qml engine
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc://.");
    Q_INIT_RESOURCE(qml);

    // load main qml file
    engine.load(QUrl(QLatin1String("qrc:/app/mainApplication.qml")));

    const bool errorCode = app.exec();

    return errorCode;
}