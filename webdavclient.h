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
    Q_PROPERTY(QString homePath READ homePath WRITE setHomePath NOTIFY homePathChanged)
    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath NOTIFY downloadPathChanged)
    Q_PROPERTY(QWebdavUrlInfo* currentItem READ currentItem NOTIFY currentItemChanged)

    QWebdav m_webdavManager;
    QUrl m_baseUrl;
    QString m_userName;
    QString m_password;
    QString m_homePath;
    QString m_downloadPath;

    QWebdavUrlInfo* m_rootItem;
    QWebdavUrlInfo* m_currentItem;

public:
    explicit WebdavClient(QObject *parent = 0);

    QString baseUrl() const;
    QString userName() const;
    QString password() const;
    QString homePath() const;
    QString downloadPath() const;

    void setBaseUrl(const QString&);
    void setUserName(const QString&);
    void setPassword(const QString&);
    void setHomePath(const QString&);
    void setDownloadPath(const QString&);

    QWebdavUrlInfo* currentItem();
    void setCurrentItem(QWebdavUrlInfo*);

protected:
    QWebdavUrlInfo* createCachePath(const QString&);
    QWebdavUrlInfo* createCacheDir(QWebdavUrlInfo*, const QString&);

public slots:
    void download(const QString& path);
    void cd(const QString& dir);
    void refresh();

protected slots:
    void replyFinished();
    void replyError(QNetworkReply::NetworkError);
    void authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);

signals:
    void baseUrlChanged(QString);
    void userNameChanged(QString);
    void passwordChanged(QString);
    void homePathChanged(QString);
    void downloadPathChanged(QString);
    void currentItemChanged(QWebdavUrlInfo*);


};

#endif // WEBDAVCLIENT_H
