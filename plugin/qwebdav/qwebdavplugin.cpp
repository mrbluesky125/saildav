#include "qwebdavplugin.h"

#include <QtQml>

#include "qquicktreeitem.h"
#include "qquicktreemodel.h"
#include "webdavfileinfo.h"
#include "webdavmodel.h"

void QWebDavPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}

void QWebDavPlugin::registerTypes(const char *uri)
{
    //@uri qwebdav
    qmlRegisterType<QQuickTreeItem>(uri, 1, 0, "QQuickTreeItem");
    qmlRegisterType<QQuickTreeModel>(uri, 1, 0, "QQuickTreeModel");
    qmlRegisterType<QWebdavUrlInfo>(uri, 1, 0, "WebdavFileInfo");
    qmlRegisterType<QWebdavModel>(uri, 1, 0, "WebdavModel");
}


