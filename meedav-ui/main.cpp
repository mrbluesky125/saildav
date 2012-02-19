#include <QtGui>
#include <QtDeclarative>
#include <QtDebug>
#include <MDeclarativeCache>

QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/meedav.log");

void myMessageOutput(QtMsgType type, const char *msg)
{
    QTextStream fs(&file);
    fs << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") << "|" << msg << "\n";
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
    qInstallMsgHandler(myMessageOutput);

    QString version = PACKAGEVERSION;

    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
    QScopedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());

    QDeclarativeEngine* engine = view->engine();
    QStringList importPathList = engine->importPathList();
    engine->setImportPathList(importPathList << "/opt/meedav/qml");

    view->rootContext()->setContextProperty("version", version);
    view->setSource( QUrl::fromLocalFile("/opt/meedav/qml/main.qml") );
    view->showFullScreen();

    int ret = app->exec();

    file.close();
    return ret;
}
