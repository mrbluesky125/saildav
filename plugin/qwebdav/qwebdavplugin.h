#ifndef EXAMPLE_PLUGIN_H
#define EXAMPLE_PLUGIN_H

#include <QQmlExtensionPlugin>

class QWebDavPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void initializeEngine(QQmlEngine * engine, const char * uri);
    void registerTypes(const char *uri);
};

#endif // EXAMPLE_PLUGIN_H

