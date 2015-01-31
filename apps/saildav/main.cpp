#include <QtGui>
#include <QtQuick>
#include <QtDebug>

#include <sailfishapp.h>

#include "qquicktreeitem.h"
#include "qquicktreemodel.h"
#include "webdavfileinfo.h"
#include "webdavmodel.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<QQuickTreeItem>("webdav", 1, 0, "QQuickTreeItem");
    qmlRegisterType<QQuickTreeModel>("webdav", 1, 0, "QQuickTreeModel");
    qmlRegisterType<QWebdavUrlInfo>("webdav", 1, 0, "WebdavFileInfo");
    qmlRegisterType<QWebdavModel>("webdav", 1, 0, "WebdavModel");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    app->setApplicationVersion("0.1.0");

    view->rootContext()->engine()->addImportPath(":/");
    view->rootContext()->engine()->addPluginPath(":/");
    view->rootContext()->setContextProperty("applicationVersion", app->applicationVersion());
    view->setSource(QUrl("qrc:/saildav.qml"));
    view->show();

    return app->exec();
}
