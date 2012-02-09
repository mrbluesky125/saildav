/* This file is part of QWebdav
 *
 * Copyright (C) 2009-2010 Corentin Chary <corentin.chary@gmail.com>
 *
 * Modified for MeeDav:
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

#ifndef QWEBDAV_URL_INFO_H
#define QWEBDAV_URL_INFO_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>

#include <QtCore>
#include <QtDeclarative>

#include "webdav.h"
#include "abstracttreeitem.h"

class QWebdavUrlInfo : public AbstractTreeItem
{
    Q_OBJECT

    Q_PROPERTY(bool dir READ isDir WRITE setDir NOTIFY dirChanged)
    Q_PROPERTY(bool file READ isFile WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)
    Q_PROPERTY(QString lastModified READ lastModified WRITE setLastModified NOTIFY lastModifiedChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(int permissions READ permissions WRITE setPermissions NOTIFY permissionsChanged)
    Q_PROPERTY(bool readable READ isReadable WRITE setReadable NOTIFY readableChanged)
    Q_PROPERTY(qint64 size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool symLink READ isSymLink WRITE setSymLink NOTIFY symLinkChanged)
    Q_PROPERTY(bool writable READ isWritable WRITE setWritable NOTIFY writableChanged)
    Q_PROPERTY(QString createdAt READ createdAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString contentLanguage READ contentLanguage WRITE setContentLanguage NOTIFY contentLanguageChanged)
    Q_PROPERTY(QString entitytag READ entitytag WRITE setEntitytag NOTIFY entitytagChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)

    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath NOTIFY downloadPathChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

private:
    QDomNode m_node;
    QWebdav::PropValues m_properties;

    bool m_dir;
    bool m_file;
    QString m_group;
    QString m_lastModified;
    QString m_name;
    QString m_owner;
    int m_permissions;
    bool m_readable;
    qint64 m_size;
    bool m_symLink;
    bool m_writable;
    QString m_createdAt;
    QString m_displayName;
    QString m_source;
    QString m_contentLanguage;
    QString m_entityTag;
    QString m_mimeType;

    QString m_downloadPath;
    qreal m_progress;
    bool m_busy;

    QNetworkReply* m_reply;

public:
    QWebdavUrlInfo(QWebdavUrlInfo* parent = 0);
    QWebdavUrlInfo(const QString& name, QWebdavUrlInfo* parent = 0);
    virtual ~QWebdavUrlInfo();

public:
    void setDir(bool b);
    void setFile(bool b);
    void setGroup(const QString & s);
    void setLastModified(const QString& dt);
    void setName(const QString & name);
    void setOwner(const QString & s);
    void setPermissions(int p);
    void setReadable(bool b);
    void setSize(qint64 size);
    void setSymLink(bool b);
    void setWritable(bool b);
    void setCreatedAt(const QString& date);
    void setDisplayName(const QString& name);
    void setSource(const QString& source);
    void setContentLanguage(const QString& lang);
    void setEntitytag(const QString& etag);
    void setMimeType(const QString& mime);

    void setDownloadPath(const QString&);

    bool isDir() const;
    bool isFile() const;
    QString group() const;
    QString lastModified() const;
    QString name() const;
    QString owner() const;
    int permissions() const;
    bool isReadable() const;
    qint64 size() const;
    bool isSymLink() const;
    bool isWritable() const;
    QString createdAt() const;
    QString displayName() const;
    QString source() const;
    QString contentLanguage() const;
    QString entitytag() const;
    QString mimeType() const;

    QString downloadPath() const;
    qreal progress() const;
    bool isBusy() const;

    QDomElement propElement() const;
    const QWebdav::PropValues & properties() const;

    void setReply(QNetworkReply*);

public slots:
    void abort();

protected slots:
    void error(QNetworkReply::NetworkError code);
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

protected:
    void setProgress(qreal progress);
    void setBusy(bool busy);

    void setMultiResponse(const QString& xmlData);
    void setResponse(const QDomElement& dom);

    int codeFromResponse(const QString& response);
    QDateTime parseDateTime(const QString& input, const QString& type);
    void davParsePropstats(const QString& path, const QDomNodeList& propstat);

signals:
    void dirChanged(bool);
    void fileChanged(bool);
    void groupChanged(QString);
    void lastModifiedChanged(QString);
    void nameChanged(QString);
    void ownerChanged(QString);
    void permissionsChanged(int);
    void readableChanged(bool);
    void sizeChanged(qint64);
    void symLinkChanged(bool);
    void writableChanged(bool);
    void createdAtChanged(QString);
    void displayNameChanged(QString);
    void sourceChanged(QString);
    void contentLanguageChanged(QString);
    void entitytagChanged(QString);
    void mimeTypeChanged(QString);

    void downloadPathChanged(QString);
    void progressChanged(qreal);
    void busyChanged(bool);
};

QML_DECLARE_TYPE(QWebdavUrlInfo)

#endif /* QWEBDAV_URL_INFO_H */
