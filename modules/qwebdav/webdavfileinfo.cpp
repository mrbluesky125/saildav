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

QWebdavUrlInfo::QWebdavUrlInfo(QWebdavUrlInfo* parent) : QQuickTreeItem(parent)
  ,m_dir(false)
  ,m_file(false)
  ,m_group()
  ,m_lastModified()
  ,m_name()
  ,m_owner()
  ,m_permissions(755)
  ,m_readable(true)
  ,m_size(0)
  ,m_symLink(false)
  ,m_writable(true)
  ,m_createdAt()
  ,m_displayName()
  ,m_source()
  ,m_contentLanguage()
  ,m_entityTag()
  ,m_mimeType()
  ,m_progress(-1)
  ,m_busy(false)
  ,m_reply(0)
{

}

QWebdavUrlInfo::QWebdavUrlInfo(const QString& name, QWebdavUrlInfo* parent) : QQuickTreeItem(parent)
  ,m_dir(false)
  ,m_file(false)
  ,m_group()
  ,m_lastModified()
  ,m_name(name)
  ,m_owner()
  ,m_permissions(755)
  ,m_readable(true)
  ,m_size(0)
  ,m_symLink(false)
  ,m_writable(true)
  ,m_createdAt()
  ,m_displayName()
  ,m_source()
  ,m_contentLanguage()
  ,m_entityTag()
  ,m_mimeType()
  ,m_progress(-1)
  ,m_busy(false)
  ,m_reply(0)
{

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
    QLocale usLocal(QLocale::English, QLocale::UnitedStates);

    if ( type == "dateTime.tz" )
        datetime =  QDateTime::fromString(input, Qt::ISODate );
    else if ( type == "dateTime.rfc1123" )
        datetime = usLocal.toDateTime( input );

    if (datetime.isValid())
        return datetime;

    datetime = usLocal.toDateTime(input.left(25), "ddd, dd MMM yyyy hh:mm:ss");
    if (datetime.isValid())
        return datetime;
    datetime = usLocal.toDateTime(input.left(19), "yyyy-MM-ddThh:mm:ss");
    if (datetime.isValid())
        return datetime;
    datetime = usLocal.toDateTime(input.mid(5, 20) , "d MMM yyyy hh:mm:ss");
    if (datetime.isValid())
        return datetime;
    QDate date;
    QTime time;

    date = usLocal.toDate(input.mid(5, 11) , "d MMM yyyy");
    time = usLocal.toTime(input.mid(17, 8) , "hh:mm:ss");
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
    QStringList pathElements = path.split('/', QString::SkipEmptyParts);
    setDisplayName(pathElements.isEmpty() ? "/" : pathElements.back());

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

            if ( property.namespaceURI() != "DAV:" ) {
                // break out - we're only interested in properties from the DAV namespace
                continue;
            }

            if ( property.tagName() == "creationdate" )
                setCreatedAt(parseDateTime( property.text(), property.attribute("dt") ).toString("dd.MM.yyyy hh:mm"));
            else if ( property.tagName() == "getcontentlength" )
                setSize(property.text().toULong());
            else if ( property.tagName() == "displayname" )
                ;//setDisplayName(property.text());
            else if ( property.tagName() == "source" )
            {
                QDomElement source;
                source = property.namedItem( "link" ).toElement().namedItem( "dst" ).toElement();

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
                setLastModified(parseDateTime( property.text(), property.attribute("dt") ).toString("dd.MM.yyyy hh:mm"));
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

void QWebdavUrlInfo::readFromJson(QJsonObject object)
{
    setDir(object.value("dir").toBool());
    setFile(object.value("file").toBool());
    setGroup(object.value("group").toString());
    setLastModified(object.value("lastModified").toString());
    setName(object.value("name").toString());
    setOwner(object.value("owner").toString());
    setPermissions(object.value("permissions").toInt());
    setReadable(object.value("readable").toBool());
    setSize(object.value("size").toInt());
    setSymLink(object.value("symLink").toBool());
    setWritable(object.value("writable").toBool());
    setCreatedAt(object.value("createdAt").toString());
    setDisplayName(object.value("displayName").toString());
    setSource(object.value("source").toString());
    setContentLanguage(object.value("contentLanguage").toString());
    setEntitytag(object.value("entitytag").toString());
    setMimeType(object.value("mimeType").toString());

    QJsonArray jsonChildList = object["childs"].toArray();
    foreach(QJsonValue jsonChild, jsonChildList) {
        QWebdavUrlInfo* newChild(new QWebdavUrlInfo(this));
        newChild->readFromJson(jsonChild.toObject());
    }
}

void QWebdavUrlInfo::writeToJson(QJsonObject& object) const
{
    object.insert("dir", isDir());
    object.insert("file", isFile());
    object.insert("group", group());
    object.insert("lastModified", lastModified());
    object.insert("name", name());
    object.insert("owner", owner());
    object.insert("permissions", QString::number(permissions()));
    object.insert("readable", isReadable());
    object.insert("size", QString::number(size()));
    object.insert("symLink", isSymLink());
    object.insert("writable", isWritable());
    object.insert("createdAt", createdAt());
    object.insert("displayName", displayName());
    object.insert("source", source());
    object.insert("contentLanguage", contentLanguage());
    object.insert("entitytag", entitytag());
    object.insert("mimeType", mimeType());

    QJsonArray jsonChildList;
    foreach(QQuickTreeItem* child, childList()) {
        jsonChildList.append(child->toJson());
    }
    object["childs"] = jsonChildList;
}

void QWebdavUrlInfo::setDir(bool b)
{
    if(m_dir == b) return;

    m_dir = b;
    emit dirChanged(b);
}

void QWebdavUrlInfo::setFile(bool b)
{
    if(m_file == b) return;

    m_file = b;
    emit fileChanged(b);
}

void QWebdavUrlInfo::setGroup(const QString& s)
{
    if(m_group == s) return;

    m_group =s;
    emit groupChanged(s);
}

void QWebdavUrlInfo::setLastModified(const QString& dt)
{
    if(m_lastModified == dt) return;

    m_lastModified = dt;
    emit lastModifiedChanged(dt);
}

void QWebdavUrlInfo::setName(const QString& name)
{
    if(m_name == name) return;

    m_name = name;
    emit nameChanged(name);
}

void QWebdavUrlInfo::setOwner(const QString& s)
{
    if(m_owner == s) return;

    m_owner = s;
    emit ownerChanged(s);
}

void QWebdavUrlInfo::setPermissions(int p)
{
    if(m_permissions == p) return;

    m_permissions = p;
    emit permissionsChanged(p);
}

void QWebdavUrlInfo::setReadable(bool b)
{
    if(m_readable == b) return;

    m_readable = b;
    emit readableChanged(b);
}

void QWebdavUrlInfo::setSize(qint64 size)
{
    if(m_size == size) return;

    m_size = size;
    emit sizeChanged(size);
}

void QWebdavUrlInfo::setSymLink(bool b)
{
    if(m_symLink == b) return;

    m_symLink = b;
    emit symLinkChanged(b);
}

void QWebdavUrlInfo::setWritable(bool b)
{
    if(m_writable == b) return;

    m_writable = b;
    emit writableChanged(b);
}

void QWebdavUrlInfo::setCreatedAt(const QString & date)
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

bool QWebdavUrlInfo::isDir() const
{
    return m_dir;
}

bool QWebdavUrlInfo::isFile() const
{
    return m_file;
}

QString QWebdavUrlInfo::group() const
{
    return m_group;
}

QString QWebdavUrlInfo::lastModified() const
{
    return m_lastModified;
}

QString QWebdavUrlInfo::name() const
{
    return m_name;
}

QString QWebdavUrlInfo::owner() const
{
    return m_owner;
}

int QWebdavUrlInfo::permissions() const
{
    return m_permissions;
}

bool QWebdavUrlInfo::isReadable() const
{
    return m_readable;
}

qint64 QWebdavUrlInfo::size() const
{
    return m_size;
}

bool QWebdavUrlInfo::isSymLink() const
{
    return m_symLink;
}

bool QWebdavUrlInfo::isWritable() const
{
    return m_writable;
}

QString QWebdavUrlInfo::createdAt() const
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

void QWebdavUrlInfo::setMultiResponse(const QString& xmlData)
{
    //get current child list
    QList<QQuickTreeItem*> oldItems = childList();

    QDomDocument multiResponse;
    multiResponse.setContent(xmlData, true);

    for(QDomNode n = multiResponse.documentElement().firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement thisResponse = n.toElement();

        if(thisResponse.isNull())
            continue;

        QString responseName = QUrl::fromPercentEncoding(thisResponse.namedItem("href").toElement().text().toUtf8());
        if(responseName.isEmpty()) continue;

        QWebdavUrlInfo* item = findFirst<QWebdavUrlInfo>(responseName, "name");
        if(item == 0) item = findFirst<QWebdavUrlInfo>(responseName + "/", "name");

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
        QCoreApplication::processEvents();
    }

    //remove not updated items
    foreach(QQuickTreeItem* item, oldItems) {
        removeChild(item);
    }
}

void QWebdavUrlInfo::setResponse(const QDomElement& dom)
{
    QDomElement href = dom.namedItem( "href" ).toElement();
    if ( href.isNull() ) return;

    QString urlStr = QUrl::fromPercentEncoding(href.text().toUtf8());
    QDomNodeList propstats = dom.elementsByTagName( "propstat" );
    davParsePropstats( urlStr, propstats );
}

void QWebdavUrlInfo::setReply(QNetworkReply* reply)
{
    if(isBusy()) {
        qWarning() << "QWebdavUrlInfo | QWebdavUrlInfo is busy.";
        return;
    }

    setProgress(0.0);
    setBusy(true);
    m_reply = reply;
    connect(m_reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
}

void QWebdavUrlInfo::abort()
{
    if(m_reply == 0) return;
    m_reply->abort();
}

void QWebdavUrlInfo::finished()
{
    QString contentType = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << "QWebdavUrlInfo | Reply finished. Content header:" << contentType;
    setBusy(false);

    if(m_reply->error() != QNetworkReply::NoError) {
        qDebug() << "QWebdavUrlInfo | Reply has error. Error:" << m_reply->errorString() << "Code:" << m_reply->error();
    }
    else {
        QByteArray data = m_reply->readAll();
        if(data.isEmpty()) {
            qDebug() << "QWebdavUrlInfo | Reply has no data."; //<< m_reply->rawHeaderPairs();
        }

        if(contentType.contains("xml")) {
            //qDebug() << data;
            setMultiResponse(data);
        }
        else if(isFile() && progress() >= 1.0) {
            qDebug() << "QWebdavUrlInfo | Download of" << displayName() << "finished.";
        }
    }

    setProgress(0.0);
    m_reply->deleteLater();
    m_reply = 0;
}

void QWebdavUrlInfo::error(QNetworkReply::NetworkError code)
{
    qDebug() << "QWebdavUrlInfo | Network error occured. Error:" << m_reply->errorString() << "Code:" << code;
}

void QWebdavUrlInfo::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qreal progress = (qreal)bytesReceived/(qreal)bytesTotal;
    setProgress(progress > 0 ? progress : 0);
    qDebug() << "QWebdavUrlInfo | Download progress." << bytesReceived << "Bytes from" << bytesTotal << "received.";
}

void QWebdavUrlInfo::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qreal progress = (qreal)bytesSent/(qreal)bytesTotal;
    setProgress(progress > 0 ? progress : bytesTotal);
    qDebug() << "QWebdavUrlInfo | Upload progress." << bytesSent << "Bytes from" << bytesTotal << "sent.";
}
