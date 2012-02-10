#ifndef Q_WEBDAV_PLUGIN_H
#define Q_WEBDAV_PLUGIN_H

#include <QtCore>
#include <QtDeclarative>

#include "webdav.h"
#include "webdavfileinfo.h"
#include "webdavmodel.h"

class QWebdavPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri)
    {
        //@uri Webdav
        qmlRegisterType<AbstractTreeItem>(uri, 1, 0, "AbstractTreeItem");
        qmlRegisterType<AbstractTreeModel>(uri, 1, 0, "AbstractTreeModel");
        qmlRegisterType<QWebdavUrlInfo>(uri, 1, 0, "WebdavFileInfo");
        qmlRegisterType<QWebdavModel>(uri, 1, 0, "WebdavModel");
    }
};

Q_EXPORT_PLUGIN2(qmlqwebdavplugin, QWebdavPlugin)

#endif
