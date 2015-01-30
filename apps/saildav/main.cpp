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
    qmlRegisterType<QQuickTreeItem>("qwebdav", 1, 0, "QQuickTreeItem");
    qmlRegisterType<QQuickTreeModel>("qwebdav", 1, 0, "QQuickTreeModel");
    qmlRegisterType<QWebdavUrlInfo>("qwebdav", 1, 0, "WebdavFileInfo");
    qmlRegisterType<QWebdavModel>("qwebdav", 1, 0, "WebdavModel");

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    view->rootContext()->engine()->addImportPath(":/");
    view->rootContext()->engine()->addPluginPath(":/");
    view->setSource(QUrl("qrc:/saildav.qml"));
    view->show();

    return app->exec();
}
