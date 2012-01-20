#include "webdavclient.h"
#include <QtDebug>

WebdavClient::WebdavClient(QObject *parent) : QObject(parent)
  ,m_currentPath("")
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));

}

QString WebdavClient::baseUrl() const
{
    return m_baseUrl.toString();
}

QString WebdavClient::userName() const
{
    return m_baseUrl.userName();
}

QString WebdavClient::password() const
{
    return m_baseUrl.password();
}

QString WebdavClient::currentPath() const
{
    return m_currentPath;
}

void WebdavClient::setBaseUrl(const QString& baseUrl)
{
    if(m_baseUrl.toString() == baseUrl) return;

    m_baseUrl.setUrl(baseUrl);
    emit baseUrlChanged(baseUrl);

    refresh();
}

void WebdavClient::setUserName(const QString& userName)
{
    if(this->userName() == userName) return;

    m_baseUrl.setUserName(userName);
    emit userNameChanged(this->userName());
}

void WebdavClient::setPassword(const QString& password)
{
    if(this->password() == password) return;

    m_baseUrl.setPassword(password);
    emit passwordChanged(this->password());
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
    return m_currentReply != 0;
}

void WebdavClient::refresh()
{
    m_currentReply = m_webdavManager.list(baseUrl() + currentPath());
    connect(m_currentReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));

}

void WebdavClient::replyFinished()
{
    QByteArray data = m_currentReply->readAll();

    m_currentReply->deleteLater();
    m_currentReply = 0;
    emit busyChanged(false);
}

void WebdavClient::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "WebdavClient | NetworkError:" << error;
    m_currentReply->deleteLater();
    m_currentReply = 0;
    emit busyChanged(false);
}

void WebdavClient::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}
