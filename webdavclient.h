#ifndef WEBDAVCLIENT_H
#define WEBDAVCLIENT_H

#include <QtCore>
#include <QtNetwork>

#include "webdav.h"
#include "webdav_url_info.h"

class WebdavClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

    QWebdav m_webdavManager;
    QUrl m_baseUrl;
    QString m_userName;
    QString m_password;
    QString m_currentPath;

    QNetworkReply* m_currentReply;
    QWebdavUrlInfo* m_rootItem;

public:
    explicit WebdavClient(QObject *parent = 0);

    QString baseUrl() const;
    QString userName() const;
    QString password() const;
    QString currentPath() const;

    void setBaseUrl(const QString&);
    void setUserName(const QString&);
    void setPassword(const QString&);
    void setCurrentPath(const QString&);

    bool busy() const;

public slots:
    void refresh();

protected slots:
    void replyFinished();
    void replyError(QNetworkReply::NetworkError);
    void authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);

signals:
    void baseUrlChanged(QString);
    void userNameChanged(QString);
    void passwordChanged(QString);
    void currentPathChanged(QString);
    void busyChanged(bool);

};

#endif // WEBDAVCLIENT_H