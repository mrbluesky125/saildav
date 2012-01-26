#include "webdavclient.h"
<<<<<<< HEAD


=======
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
#include <QtDebug>

WebdavClient::WebdavClient(QObject *parent) : QObject(parent)
  ,m_currentPath("")
<<<<<<< HEAD
  ,m_userName("")
  ,m_password("")
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
    m_currentItem = m_rootItem = new QWebdavUrlInfo(this);
=======
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));

>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

QString WebdavClient::baseUrl() const
{
<<<<<<< HEAD
    return m_baseUrl.toString(QUrl::RemovePath);
=======
    return m_baseUrl.toString();
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

QString WebdavClient::userName() const
{
<<<<<<< HEAD
    return m_userName;
=======
    return m_baseUrl.userName();
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

QString WebdavClient::password() const
{
<<<<<<< HEAD
    return m_password;
=======
    return m_baseUrl.password();
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

QString WebdavClient::currentPath() const
{
    return m_currentPath;
}

void WebdavClient::setBaseUrl(const QString& baseUrl)
{
<<<<<<< HEAD
    //if(m_baseUrl.toString() == baseUrl) return;
=======
    if(m_baseUrl.toString() == baseUrl) return;
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a

    m_baseUrl.setUrl(baseUrl);
    emit baseUrlChanged(baseUrl);

<<<<<<< HEAD
    setCurrentPath(m_baseUrl.path());
=======
    refresh();
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

void WebdavClient::setUserName(const QString& userName)
{
<<<<<<< HEAD
    if(m_userName == userName) return;

    m_userName = userName;
    emit userNameChanged(m_userName);
=======
    if(this->userName() == userName) return;

    m_baseUrl.setUserName(userName);
    emit userNameChanged(this->userName());
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

void WebdavClient::setPassword(const QString& password)
{
<<<<<<< HEAD
    if(m_password == password) return;

    m_password = password;
    emit passwordChanged(m_password);
=======
    if(this->password() == password) return;

    m_baseUrl.setPassword(password);
    emit passwordChanged(this->password());
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
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
<<<<<<< HEAD
    return true;
=======
    return m_currentReply != 0;
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

void WebdavClient::refresh()
{
<<<<<<< HEAD
    qDebug() << "WebdavClient | List url:" << baseUrl() + currentPath();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + currentPath());
    connect(reply, SIGNAL(finished()), m_rootItem, SLOT(finished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), m_rootItem, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), m_rootItem, SLOT(downloadProgress(qint64,qint64)));

    //client handling - maybe removed
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
=======
    m_currentReply = m_webdavManager.list(baseUrl() + currentPath());
    connect(m_currentReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));

>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
}

void WebdavClient::replyFinished()
{
<<<<<<< HEAD


=======
    QByteArray data = m_currentReply->readAll();

    m_currentReply->deleteLater();
    m_currentReply = 0;
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
    emit busyChanged(false);
}

void WebdavClient::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "WebdavClient | NetworkError:" << error;
<<<<<<< HEAD

=======
    m_currentReply->deleteLater();
    m_currentReply = 0;
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
    emit busyChanged(false);
}

void WebdavClient::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
<<<<<<< HEAD
    qDebug() << "WebdavClient | Authentification for" << m_baseUrl.toString() << "required";
    qDebug() << "WebdavClient | Login with user:" << userName();
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}

QWebdavUrlInfo* WebdavClient::currentItem()
{
    return m_currentItem;
}
=======
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}
>>>>>>> 3377a1c28989e4702eddc49df85df93beb09578a
