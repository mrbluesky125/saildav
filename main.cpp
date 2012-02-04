#include <QtGui>
#include <QtDeclarative>
#include <QtDebug>
#include "qmlapplicationviewer.h"

#include "abstracttreeitem.h"
#include "abstracttreemodel.h"
#include "webdavfileinfo.h"
#include "webdavclient.h"

QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/meedav.log");

void myMessageOutput(QtMsgType type, const char *msg)
{
    QTextStream fs(&file);
    fs << msg;
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    qInstallMsgHandler(myMessageOutput);

    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    qmlRegisterType<QWebdavUrlInfo>("MeeDav", 1, 0, "WebdavUrlInfo");
    qmlRegisterType<AbstractTreeItem>("MeeDav", 1, 0, "AbstractTreeItem");

    WebdavClient client;

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->rootContext()->setContextProperty("webdavClient", &client);
    viewer->setMainQmlFile(QLatin1String("qml/meedav/main.qml"));
    viewer->showExpanded();

    int ret = app->exec();
    file.close();
    return ret;
}
