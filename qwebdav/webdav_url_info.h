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

private:
    QDomNode node_;
    QWebdav::PropValues properties_;

    QDateTime createdAt_;
    QString displayName_;
    QString source_;
    QString contentLanguage_;
    QString entityTag_;
    QString mimeType_;

public:
    QWebdavUrlInfo(QObject* parent = NULL);
    QWebdavUrlInfo(const QDomElement& dom, QObject* parent = NULL);
    QWebdavUrlInfo(const QWebdavUrlInfo& wui, QObject* parent = NULL);

    virtual ~QWebdavUrlInfo ();

    static QList<QWebdavUrlInfo*> parseListReply(const QByteArray& data);

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

    QDomElement propElement() const;
    const QWebdav::PropValues & properties() const;

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

};

#endif /* QWEBDAV_URL_INFO_H */
