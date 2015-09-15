/* This file is part of QWebdav
 *
 * Copyright (C) 2012 Timo Zimmermann <meedav@timozimmermann.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef WEBDAVMODEL_H
#define WEBDAVMODEL_H

#include <QtQuick>

#include "webdav.h"
#include "qquicktreemodel.h"

class QWebdavUrlInfo;

class QWebdavModel : public QQuickTreeModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)
    Q_PROPERTY(QString parentFolder READ parentFolder NOTIFY folderChanged)
    Q_PROPERTY(QWebdavUrlInfo* currentItem READ currentItem NOTIFY folderChanged)

    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString homePath READ homePath NOTIFY baseUrlChanged)

    QWebdav m_webdavManager;
    QString m_folder;
    QUrl m_baseUrl;
    QString m_homePath;
    QString m_localRootPath;

    bool m_refreshFlag;

public:
    QWebdavModel(QObject *parent = 0);
    ~QWebdavModel();

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

    void classBegin();
    void componentComplete();

    Q_INVOKABLE void setupDatabase();

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
    QWebdavUrlInfo* findItem(const QString& path) const;
    QString rectifyPath(const QString& path);

    QString localRootPath() const;
    void setLocalRootPath(const QString& path);

    bool loadCache(const QString& path);
    bool saveCache(const QString& path);

protected slots:
    void replyFinished();
    void replyError(QNetworkReply::NetworkError);
    void replySslError(const QList<QSslError>& errors);
    void authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);

signals:
    void folderChanged();
    void errorChanged(QString error);

    void baseUrlChanged();
    void userNameChanged();
    void passwordChanged();
};

#endif // WEBDAVMODEL_H
