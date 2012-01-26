



#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "webdavfileinfo.h"
#include "webdavclient.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    qmlRegisterType<QWebdavUrlInfo>("MeeDav", 1, 0, "WebdavUrlInfo");

    WebdavClient client;

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->rootContext()->setContextProperty("webdavClient", &client);
    viewer->setMainQmlFile(QLatin1String("qml/meedav/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
