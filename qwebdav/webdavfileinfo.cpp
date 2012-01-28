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

#include <QUrl>
#include <QDebug>

#include "webdavfileinfo.h"

QWebdavUrlInfo::QWebdavUrlInfo(QWebdavUrlInfo* parent) : AbstractTreeItem(parent)
{

}

QWebdavUrlInfo::QWebdavUrlInfo(const QString& name, QWebdavUrlInfo* parent) : AbstractTreeItem(parent)
{
    QUrlInfo::setName(name);
}

QWebdavUrlInfo::~QWebdavUrlInfo()
{

}

int QWebdavUrlInfo::codeFromResponse( const QString& response )
{
    int firstSpace = response.indexOf( ' ' );
    int secondSpace = response.indexOf( ' ', firstSpace + 1 );
    return response.mid( firstSpace + 1, secondSpace - firstSpace - 1 ).toInt();
}

QDateTime QWebdavUrlInfo::parseDateTime( const QString& input, const QString& type )
{
    QDateTime datetime;

    if ( type == "dateTime.tz" )
        datetime =  QDateTime::fromString( input, Qt::ISODate );
    else if ( type == "dateTime.rfc1123" )
        datetime = QDateTime::fromString( input );

    if (datetime.isValid())
        return datetime;

    datetime = QDateTime::fromString(input.left(25), "ddd, dd MMM yyyy hh:mm:ss");
    if (datetime.isValid())
        return datetime;
    datetime = QDateTime::fromString(input.left(19), "yyyy-MM-ddThh:mm:ss");
    if (datetime.isValid())
        return datetime;
    datetime = QDateTime::fromString(input.mid(5, 20) , "d MMM yyyy hh:mm:ss");
    if (datetime.isValid())
        return datetime;
    QDate date;
    QTime time;

    date = QDate::fromString(input.mid(5, 11) , "d MMM yyyy");
    time = QTime::fromString(input.mid(17, 8) , "hh:mm:ss");
    datetime = QDateTime(date, time);

    if(!datetime.isValid())
        qDebug() << "QWebdavUrlInfo | Unknown date time format:" << input;

    return datetime;
}

void QWebdavUrlInfo::davParsePropstats( const QString & path, const QDomNodeList & propstats )
{
    QString mimeType;
    bool foundExecutable = false;
    bool isDirectory = false;

    setName(path);

    for ( int i = 0; i < propstats.count(); i++) {
        QDomElement propstat = propstats.item(i).toElement();
        QDomElement status = propstat.namedItem( "status" ).toElement();

        if ( status.isNull() ) {
            qDebug() << "Error, no status code in this propstat";
            return;
        }

        int code = codeFromResponse( status.text() );

        if (code == 404)
            continue ;

        QDomElement prop = propstat.namedItem( "prop" ).toElement();

        if ( prop.isNull() ) {
            qDebug() << "Error: no prop segment in this propstat.";
            return;
        }

        for ( QDomNode n = prop.firstChild(); !n.isNull(); n = n.nextSibling() ) {
            QDomElement property = n.toElement();

            if (property.isNull())
                continue;

            m_properties[property.namespaceURI()][property.tagName()] = property.text();

            if ( property.namespaceURI() != "DAV:" ) {
                // break out - we're only interested in properties from the DAV namespace
                continue;
            }

            if ( property.tagName() == "creationdate" )
                setCreatedAt(parseDateTime( property.text(), property.attribute("dt") ));
            else if ( property.tagName() == "getcontentlength" )
                setSize(property.text().toULong());
            else if ( property.tagName() == "displayname" )
                setDisplayName(property.text());
            else if ( property.tagName() == "source" )
            {
                QDomElement source;

                source = property.namedItem( "link" ).toElement()
                         .namedItem( "dst" ).toElement();

                if ( !source.isNull() )
                    setSource(source.text());
            }
            else if ( property.tagName() == "getcontentlanguage" )
                setContentLanguage(property.text());
            else if ( property.tagName() == "getcontenttype" )
            {
                if ( property.text() == "httpd/unix-directory" )
                    isDirectory = true;

                mimeType = property.text();
            }
            else if ( property.tagName() == "executable" )
            {
                if ( property.text() == "T" )
                    foundExecutable = true;
            }
            else if ( property.tagName() == "getlastmodified" )
                setLastModified(parseDateTime( property.text(), property.attribute("dt") ));
            else if ( property.tagName() == "getetag" )
                setEntitytag(property.text());
            else if ( property.tagName() == "resourcetype" )
            {
                if ( !property.namedItem( "collection" ).toElement().isNull() )
                    isDirectory = true;
            }
            else
                qDebug() << "Found unknown webdav property: "
                         << property.tagName() << property.text();
        }
    }
    setDir(isDirectory);
    setFile(!isDirectory);

    if (isDirectory && !name().endsWith("/"))
        setName(name() + "/");

    if ( foundExecutable || isDirectory )
        setPermissions(0700);
    else
        setPermissions(0600);

    if ( !isDirectory && !mimeType.isEmpty() )
        setMimeType(mimeType);
}

void QWebdavUrlInfo::setDir(bool b)
{
    if(isDir() == b) return;

    QUrlInfo::setDir(b);
    emit dirChanged(b);
}

void QWebdavUrlInfo::setFile(bool b)
{
    if(isFile() == b) return;

    QUrlInfo::setFile(b);
    emit fileChanged(b);
}

void QWebdavUrlInfo::setGroup(const QString& s)
{
    if(group() == s) return;

    QUrlInfo::setGroup(s);
    emit groupChanged(s);
}

void QWebdavUrlInfo::setLastModified(const QDateTime& dt)
{
    if(lastModified() == dt) return;

    QUrlInfo::setLastModified(dt);
    emit lastModifiedChanged(dt);
}

void QWebdavUrlInfo::setName(const QString& name)
{
    if(QUrlInfo::name() == name) return;

    QUrlInfo::setName(name);
    emit nameChanged(name);

    if(name.isEmpty()) return;
    setDisplayName(name.split('/', QString::SkipEmptyParts).back());
}

void QWebdavUrlInfo::setOwner(const QString& s)
{
    if(owner() == s) return;

    QUrlInfo::setOwner(s);
    emit ownerChanged(s);
}

void QWebdavUrlInfo::setPermissions(int p)
{
    if(permissions() == p) return;

    QUrlInfo::setPermissions(p);
    emit permissionsChanged(p);
}

void QWebdavUrlInfo::setReadable(bool b)
{
    if(isReadable() == b) return;

    QUrlInfo::setReadable(b);
    emit readableChanged(b);
}

void QWebdavUrlInfo::setSize(qint64 size)
{
    if(QUrlInfo::size() == size) return;

    QUrlInfo::setSize(size);
    emit sizeChanged(size);
}

void QWebdavUrlInfo::setSymLink(bool b)
{
    if(isSymLink() == b) return;

    QUrlInfo::setSymLink(b);
    emit symLinkChanged(b);
}

void QWebdavUrlInfo::setWritable(bool b)
{
    if(isWritable() == b) return;

    QUrlInfo::setWritable(b);
    emit writableChanged(b);
}

void QWebdavUrlInfo::setCreatedAt(const QDateTime & date)
{
    if(m_createdAt == date) return;

    m_createdAt = date;
    emit createdAtChanged(m_createdAt);
}

void QWebdavUrlInfo::setDisplayName(const QString & name)
{
    if(m_displayName == name) return;

    m_displayName = name;
    emit displayNameChanged(m_displayName);
}

void QWebdavUrlInfo::setSource(const QString & source)
{
    if(m_source == source) return;

    m_source = source;
    emit sourceChanged(m_source);
}

void QWebdavUrlInfo::setContentLanguage(const QString & lang)
{
    if( m_contentLanguage == lang) return;

    m_contentLanguage = lang;
    emit contentLanguageChanged(m_contentLanguage);
}

void QWebdavUrlInfo::setEntitytag(const QString & etag)
{
    if(m_entityTag == etag) return;

    m_entityTag = etag;
    emit entitytagChanged(m_entityTag);
}

void QWebdavUrlInfo::setMimeType(const QString & mime)
{
    if(m_mimeType == mime) return;

    m_mimeType = mime;
    emit mimeTypeChanged(m_mimeType);
}

void QWebdavUrlInfo::setProgress(qreal progress)
{
    if(qFuzzyCompare(m_progress,progress)) return;

    m_progress = progress;
    emit progressChanged(progress);
}

void QWebdavUrlInfo::setBusy(bool busy)
{
    if(m_busy == busy) return;

    m_busy = busy;
    emit busyChanged(busy);
}

QDateTime QWebdavUrlInfo::createdAt() const
{
    return m_createdAt;
}

QString QWebdavUrlInfo::displayName() const
{
    return m_displayName;
}

QString QWebdavUrlInfo::source() const
{
    return m_source;
}

QString QWebdavUrlInfo::contentLanguage() const
{
    return m_contentLanguage;
}

QString QWebdavUrlInfo::entitytag() const
{
    return m_entityTag;
}

QString QWebdavUrlInfo::mimeType() const
{
    return m_mimeType;
}

qreal QWebdavUrlInfo::progress() const
{
    return m_progress;
}

bool QWebdavUrlInfo::isBusy() const
{
    return m_busy;
}

QDomElement QWebdavUrlInfo::propElement() const
{
    return m_node.toElement();
}

const QWebdav::PropValues& QWebdavUrlInfo::properties() const
{
    return m_properties;
}

void QWebdavUrlInfo::setMultiResponse(const QString& xmlData)
{
    //get current child list
    QList<AbstractTreeItem*> oldItems = childList();

    QDomDocument multiResponse;
    multiResponse.setContent(xmlData, true);

    for(QDomNode n = multiResponse.documentElement().firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement thisResponse = n.toElement();

        if(thisResponse.isNull())
            continue;

        QString responseName = QUrl::fromPercentEncoding(thisResponse.namedItem("href").toElement().text().toUtf8());
        if(responseName.isEmpty()) continue;

        QWebdavUrlInfo* item = static_cast<QWebdavUrlInfo*>(findFirst(responseName, "name"));

        if(item != 0) { //item is already in the list - update only
            item->setResponse(thisResponse);
            oldItems.removeAll(item);
        }
        else { //add new item
            item = new QWebdavUrlInfo();
            item->setResponse(thisResponse);
            addChild(item);
        }

        qDebug() << "QWebdavUrlInfo | Response received: " << item->name();
    }

    //remove not updated items
    foreach(AbstractTreeItem* item, oldItems) {
        removeChild(item);
    }
}

void QWebdavUrlInfo::setResponse(const QDomElement& dom)
{
    QDomElement href = dom.namedItem( "href" ).toElement();
    m_node = dom.cloneNode();

    if ( !href.isNull() )
    {
        QString urlStr = QUrl::fromPercentEncoding(href.text().toUtf8());
        QDomNodeList propstats = dom.elementsByTagName( "propstat" );
        davParsePropstats( urlStr, propstats );
    }
}

void QWebdavUrlInfo::finished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if(reply == 0) {
        qDebug() << "QWebdavUrlInfo | Reply seems not to be a QNetworkReply object. Reply is ignored.";
        return;
    }

    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << "QWebdavUrlInfo | Reply content header:" << contentType;

    QByteArray data = reply->readAll();
    if(data.isEmpty()) {
        qDebug() << "QWebdavUrlInfo | Reply has no data. Reply is ignored.";
        return;
    }

    if(contentType.contains("xml"))
        setMultiResponse(data);
    else if(isFile()) {
        qDebug() << "QWebdavUrlInfo | File download finished.";
        QFile file(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/" + displayName());
        file.open(QFile::WriteOnly);
        file.write(data);
        file.close();
    }

    setBusy(false);
    reply->deleteLater();
}

void QWebdavUrlInfo::error(QNetworkReply::NetworkError code)
{
    qDebug() << "QWebdavUrlInfo | Network error occured. Error code:" << code;
    setBusy(false);
}

void QWebdavUrlInfo::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    setBusy(true);
    qreal progress = (qreal)bytesReceived/(qreal)bytesTotal;
    setProgress(progress > 0 ? progress : bytesTotal);
    qDebug() << "QWebdavUrlInfo | Download progress." << bytesReceived << "Bytes from" << bytesTotal << "received.";
}
