#include "exampleqmlplugin.h"

#include <QtQml>

void ExamplePlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{

}

void ExamplePlugin::registerTypes(const char *uri)
{
    //replace the uri with the current plugin name, this is used by the qml editor of QtCreator
    //to resolve the registered plugin types

    //@uri example
    //qmlRegisterType<MyItem>(uri, 1, 0, "MyItem");

    //@uri qwebdav
    qmlRegisterType<AbstractTreeItem>(uri, 1, 0, "AbstractTreeItem");
    qmlRegisterType<AbstractTreeModel>(uri, 1, 0, "AbstractTreeModel");
    qmlRegisterType<QWebdavUrlInfo>(uri, 1, 0, "WebdavFileInfo");
    qmlRegisterType<QWebdavModel>(uri, 1, 0, "WebdavModel");
}


