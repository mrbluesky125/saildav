#include "webdavclient.h"
#include <QtDebug>

WebdavClient::WebdavClient(QObject *parent) : QObject(parent)
  ,m_homePath("/")
  ,m_userName("")
  ,m_password("")
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
    m_currentItem = m_rootItem = new QWebdavUrlInfo("/");
}

QString WebdavClient::baseUrl() const
{
    return m_baseUrl.toString(QUrl::RemovePath);
}

QString WebdavClient::userName() const
{
    return m_userName;
}

QString WebdavClient::password() const
{
    return m_password;
}

QString WebdavClient::homePath() const
{
    return m_homePath;
}

void WebdavClient::setBaseUrl(const QString& baseUrl)
{
    //if(m_baseUrl.toString() == baseUrl) return;

    m_baseUrl.setUrl(baseUrl);
    emit baseUrlChanged(baseUrl);

    setHomePath(m_baseUrl.path());
    cd(homePath());
}

void WebdavClient::setUserName(const QString& userName)
{
    if(m_userName == userName) return;

    m_userName = userName;
    emit userNameChanged(m_userName);
}

void WebdavClient::setPassword(const QString& password)
{
    if(m_password == password) return;

    m_password = password;
    emit passwordChanged(m_password);
}

void WebdavClient::setHomePath(const QString& homePath)
{
    if(m_homePath == homePath) return;

    m_homePath = homePath;
    emit homePathChanged(m_homePath);
}

void WebdavClient::replyFinished()
{

}

void WebdavClient::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "WebdavClient | NetworkError:" << error;
}

void WebdavClient::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    qDebug() << "WebdavClient | Authentification for" << m_baseUrl.toString() << "required";
    qDebug() << "WebdavClient | Login with user:" << userName();
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}

QWebdavUrlInfo* WebdavClient::currentItem()
{
    return m_currentItem;
}

void WebdavClient::setCurrentItem(QWebdavUrlInfo* currentItem)
{
    if(m_currentItem == currentItem) return;

    m_currentItem = currentItem;
    emit currentItemChanged(m_currentItem);
}

QWebdavUrlInfo* WebdavClient::createCachePath(const QString& path)
{
    qDebug() << "WebdavClient | Create cache path:" << path;

    if(!path.startsWith('/')) {
        qDebug() << "WebdavClient | Cannot create the relative path:" << path;
        return m_rootItem;
    }

    //check for existing item
    QWebdavUrlInfo* currentItem = static_cast<QWebdavUrlInfo*>(m_rootItem->findFirst(path, "name"));
    if(currentItem != 0)
        return currentItem;

    QStringList dirNames = path.split('/', QString::SkipEmptyParts);
    currentItem = m_rootItem;
    foreach(QString dir, dirNames) {
        if(dir == ".") continue;
        if(dir == "..") {
            currentItem = static_cast<QWebdavUrlInfo*>(currentItem->parentItem() != 0 ? currentItem->parentItem() : currentItem);
            continue;
        }
        currentItem = createCacheDir(currentItem, dir);
    }
    return currentItem;
}

QWebdavUrlInfo* WebdavClient::createCacheDir(QWebdavUrlInfo* parentItem, const QString& dirName)
{
    if(parentItem == 0 || dirName.isEmpty()) return 0;

    //search
    foreach(AbstractTreeItem* item, parentItem->childList()) {
        QWebdavUrlInfo* urlInfoItem = static_cast<QWebdavUrlInfo*>(item);
        if(urlInfoItem->name().endsWith(dirName + "/"))
            return urlInfoItem;
    }

    //create
    QWebdavUrlInfo* urlInfoItem = new QWebdavUrlInfo();
    urlInfoItem->setName(parentItem->name() + dirName + "/");
    urlInfoItem->setDir(true);
    parentItem->addChild(urlInfoItem);

    qDebug() << "WebdavClient | Create cache directory:" << urlInfoItem->name();

    return urlInfoItem;
}

void WebdavClient::remove(const QString& path)
{
    //I assume the path is already in the cache, if not, nothing happens
    QWebdavUrlInfo* currentItem = static_cast<QWebdavUrlInfo*>(m_rootItem->findFirst(path, "name"));
    if(currentItem == 0) {
        qDebug() << "WebdavClient | Cannot remove entry, not found:" << currentItem->name();
        return;
    }

    QNetworkReply* reply = m_webdavManager.remove(baseUrl() + path);
    currentItem->connectReply(reply);
}

void WebdavClient::upload(const QString& path, const QString& from)
{

}

void WebdavClient::mkdir(const QString& path)
{
    QWebdavUrlInfo* currentItem = createCachePath(path);
    if(currentItem == m_rootItem) {
        qDebug() << "WebdavClient | Failed to create new folder:" << path;
        return;
    }

    QNetworkReply* reply = m_webdavManager.mkdir(baseUrl() + path);
    currentItem->connectReply(reply);
}

void WebdavClient::download(const QString& path)
{
    QWebdavUrlInfo* currentItem = createCachePath(path);
    if(currentItem == m_rootItem) {
        qDebug() << "WebdavClient | Failed to download file:" << path;
        return;
    }

    currentItem->setDownloadPath(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QNetworkReply* reply = m_webdavManager.get(baseUrl() + path);
    currentItem->connectReply(reply);
}

void WebdavClient::cd(const QString& dir)
{
    qDebug() << "WebdavClient | cd:" << dir;
    if(dir.startsWith("/"))
        setCurrentItem(createCachePath(dir));
    else
        setCurrentItem(createCachePath(m_currentItem->name() + dir));

    refresh();
}

void WebdavClient::refresh()
{
    qDebug() << "WebdavClient | List url:" << baseUrl() + m_currentItem->name();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + m_currentItem->name());
    m_currentItem->connectReply(reply);
}

