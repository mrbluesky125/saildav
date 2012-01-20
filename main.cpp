#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "webdav_url_info.h"
#include "webdavclient.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    WebdavClient client;

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->rootContext()->setContextProperty("webdavClient", &client);
    viewer->setMainQmlFile(QLatin1String("qml/meedav/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
