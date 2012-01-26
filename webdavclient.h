#ifndef WEBDAVCLIENT_H
#define WEBDAVCLIENT_H

#include <QtCore>
#include <QtNetwork>

#include "webdav.h"
#include "webdavfileinfo.h"

class WebdavClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QWebdavUrlInfo* currentItem READ currentItem NOTIFY currentItemChanged)

    QWebdav m_webdavManager;
    QUrl m_baseUrl;
    QString m_userName;
    QString m_password;
    QString m_currentPath;

    QWebdavUrlInfo* m_rootItem;
    QWebdavUrlInfo* m_currentItem;

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

    QWebdavUrlInfo* currentItem();
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
    void currentItemChanged(QWebdavUrlInfo*);

};

#endif // WEBDAVCLIENT_H
