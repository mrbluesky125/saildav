/* This file is part of QWebdav
 *
 * Copyright (C) 2009-2010 Corentin Chary <corentin.chary@gmail.com>
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
#include <QDateTime>
#include <QUrlInfo>
#include <QVariant>
#include <QtCore>
#include <QtDeclarative>

#include "webdav.h"

class QWebdavUrlInfo : public QObject, virtual public QUrlInfo
{
    Q_OBJECT

    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString contentLanguage READ contentLanguage WRITE setContentLanguage NOTIFY contentLanguageChanged)
    Q_PROPERTY(QString entitytag READ entitytag WRITE setEntitytag NOTIFY entitytagChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)

    Q_PROPERTY(QDeclarativeListProperty<QWebdavUrlInfo> items READ items NOTIFY itemsChanged)

private:
    QDomNode m_node;
    QWebdav::PropValues m_properties;

    QDateTime m_createdAt;
    QString m_displayName;
    QString m_source;
    QString m_contentLanguage;
    QString m_entityTag;
    QString m_mimeType;

    QList<QWebdavUrlInfo*> m_items;

public:
    QWebdavUrlInfo(QObject* parent = NULL);
    QWebdavUrlInfo(const QDomElement& dom, QObject* parent = NULL);

    virtual ~QWebdavUrlInfo();

public:
    void setCreatedAt(const QDateTime& date);
    void setDisplayName(const QString& name);
    void setSource(const QString& source);
    void setContentLanguage(const QString& lang);
    void setEntitytag(const QString& etag);
    void setMimeType(const QString& mime);

    QDateTime createdAt() const;
    QString displayName() const;
    QString source() const;
    QString contentLanguage() const;
    QString entitytag() const;
    QString mimeType() const;

    QDeclarativeListProperty<QWebdavUrlInfo> items();
    static void append(QDeclarativeListProperty<QWebdavUrlInfo>* list, QWebdavUrlInfo* item);
    static int count(QDeclarativeListProperty<QWebdavUrlInfo>* list);
    static QWebdavUrlInfo* at(QDeclarativeListProperty<QWebdavUrlInfo>* list, int index);

    QDomElement propElement() const;
    const QWebdav::PropValues & properties() const;

public slots:
    void error(QNetworkReply::NetworkError code);
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);


protected:
    int codeFromResponse(const QString& response);
    QDateTime parseDateTime(const QString& input, const QString& type);
    void davParsePropstats(const QString& path, const QDomNodeList& propstat);

signals:
    void createdAtChanged(QDateTime);
    void displayNameChanged(QString);
    void sourceChanged(QString);
    void contentLanguageChanged(QString);
    void entitytagChanged(QString);
    void mimeTypeChanged(QString);

    void itemsChanged(QDeclarativeListProperty<QWebdavUrlInfo>);

};

#endif /* QWEBDAV_URL_INFO_H */
