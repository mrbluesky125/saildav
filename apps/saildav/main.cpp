#include <QtGui>
#include <QtQuick>
#include <QtDebug>

#ifdef SAILFISH
#include <sailfishapp.h>
#endif

#include "qquicktreeitem.h"
#include "qquicktreemodel.h"
#include "webdavfileinfo.h"
#include "webdavmodel.h"

int main(int argc, char *argv[])
{
    //QGuiApplication* app = SailfishApp::application(int, char *[]);
    //QQuickView* view + SailfishApp::createView();

    QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
    QScopedPointer<QQuickView> view(new QQuickView());
    view->resize(768, 1024);

    qmlRegisterType<QQuickTreeItem>("qwebdav", 1, 0, "QQuickTreeItem");
    qmlRegisterType<QQuickTreeModel>("qwebdav", 1, 0, "QQuickTreeModel");
    qmlRegisterType<QWebdavUrlInfo>("qwebdav", 1, 0, "WebdavFileInfo");
    qmlRegisterType<QWebdavModel>("qwebdav", 1, 0, "WebdavModel");

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->rootContext()->engine()->addImportPath(":/");
    view->rootContext()->engine()->addPluginPath(":/");
    view->rootContext()->engine()->addImportPath("../../plugin");
    view->rootContext()->engine()->addPluginPath("../../plugin");
    view->setSource(QUrl("qrc:/Mainwindow.qml"));
    view->show();

    return app->exec();
}
