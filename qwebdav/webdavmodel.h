#ifndef WEBDAVMODEL_H
#define WEBDAVMODEL_H

#include <QtDeclarative>

#include "abstracttreemodel.h"
#include "webdav.h"

class QWebdavUrlInfo;

class QWebdavModel : public AbstractTreeModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)

    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QString parentFolder READ parentFolder NOTIFY folderChanged)
    Q_PROPERTY(QWebdavUrlInfo* currentItem READ currentItem NOTIFY folderChanged)

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString homePath READ homePath WRITE setHomePath NOTIFY homePathChanged)

    QWebdav m_webdavManager;
    QString m_folder;
    QUrl m_baseUrl;
    QString m_userName;
    QString m_password;
    QString m_homePath;

    bool m_refreshFlag;

public:
    QWebdavModel(QObject *parent = 0);

    enum Roles { FileNameRole = Qt::UserRole+1, FilePathRole = Qt::UserRole+2 };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString folder() const;
    void setFolder(const QString &path);

    QString parentFolder() const;
    QWebdavUrlInfo* currentItem() const;

    QString baseUrl() const;
    QString userName() const;
    QString password() const;
    QString homePath() const;

    void setBaseUrl(const QString&);
    void setUserName(const QString&);
    void setPassword(const QString&);
    void setHomePath(const QString&);

    void classBegin();
    void componentComplete();

public slots:
    void rename(const QString& path, const QString& to);
    void remove(const QString& path);
    void upload(const QString& path, const QString& from);
    void mkdir(const QString& path);
    void download(const QString& path);
    void cd(const QString& dir);
    void refresh();

protected:
    void connectReply(QNetworkReply*);
    QString parentFolder(const QString& path) const;
    bool createPath(const QString& path);
    QString createFolder(const QString& path, const QString& dirName);
    QWebdavUrlInfo* item(const QString& path) const;

protected slots:
    void replyFinished();
    void replyError(QNetworkReply::NetworkError);
    void authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);

signals:
    void folderChanged();
    void errorChanged(QString error);

    void baseUrlChanged(QString);
    void userNameChanged(QString);
    void passwordChanged(QString);
    void homePathChanged(QString);
};

QML_DECLARE_TYPE(QWebdavModel)

#endif // WEBDAVMODEL_H
