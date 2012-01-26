#include "webdavclient.h"


#include <QtDebug>

WebdavClient::WebdavClient(QObject *parent) : QObject(parent)
  ,m_currentPath("")
  ,m_userName("")
  ,m_password("")
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
    m_currentItem = m_rootItem = new QWebdavUrlInfo(this);
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

QString WebdavClient::currentPath() const
{
    return m_currentPath;
}

void WebdavClient::setBaseUrl(const QString& baseUrl)
{
    //if(m_baseUrl.toString() == baseUrl) return;

    m_baseUrl.setUrl(baseUrl);
    emit baseUrlChanged(baseUrl);

    setCurrentPath(m_baseUrl.path());
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

void WebdavClient::setCurrentPath(const QString& currentPath)
{
    if(m_currentPath == currentPath) return;

    m_currentPath = currentPath;
    emit currentPathChanged(m_currentPath);

    refresh();
}

bool WebdavClient::busy() const
{
    return true;
}

void WebdavClient::refresh()
{
    qDebug() << "WebdavClient | List url:" << baseUrl() + currentPath();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + currentPath());
    connect(reply, SIGNAL(finished()), m_rootItem, SLOT(finished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), m_rootItem, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), m_rootItem, SLOT(downloadProgress(qint64,qint64)));

    //client handling - maybe removed
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
}

void WebdavClient::replyFinished()
{


    emit busyChanged(false);
}

void WebdavClient::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "WebdavClient | NetworkError:" << error;

    emit busyChanged(false);
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
