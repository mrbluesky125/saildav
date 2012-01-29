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

#include <QtNetwork>
#include <QtDebug>

#include "webdav.h"
#include "webdavfileinfo.h"

QWebdav::QWebdav (QObject *parent) : QNetworkAccessManager(parent)
{
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

QWebdav::~QWebdav()
{

}

void QWebdav::replyReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QIODevice* dataIO = m_inDataDevices.value(reply, 0);
    if(dataIO == 0) return;
    dataIO->write(reply->readAll());
}

void QWebdav::replyFinished(QNetworkReply* reply)
{
    delete m_outDataDevices.value(reply, 0);
    delete m_inDataDevices.value(reply, 0);
    m_outDataDevices.remove(reply);
    m_inDataDevices.remove(reply);
}

QNetworkReply* QWebdav::createRequest(const QString& method, QNetworkRequest& req, QIODevice* outgoingData)
{
     if(outgoingData != 0 && outgoingData->size() !=0) {
        req.setHeader(QNetworkRequest::ContentLengthHeader, outgoingData->size());
        req.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    }

    return sendCustomRequest(req, method.toAscii(), outgoingData);
}

QNetworkReply* QWebdav::createRequest(const QString& method, QNetworkRequest& req, const QByteArray& outgoingData )
{
    QBuffer* dataIO = new QBuffer;
    dataIO->setData(outgoingData);
    dataIO->open(QIODevice::ReadOnly);

    QNetworkReply* reply = createRequest(method, req, dataIO);
    m_outDataDevices.insert(reply, dataIO);
    return reply;
}

QNetworkReply* QWebdav::list(const QString& path)
{
    QWebdav::PropNames query;
    QStringList props;

    props << "creationdate";
    props << "getcontentlength";
    props << "displayname";
    props << "source";
    props << "getcontentlanguage";
    props << "getcontenttype";
    props << "executable";
    props << "getlastmodified";
    props << "getetag";
    props << "resourcetype";

    query["DAV:"] = props;

    return propfind(path, query, 1);
}

QNetworkReply* QWebdav::search(const QString& path, const QString& q )
{
    QByteArray query = "<?xml version=\"1.0\"?>\r\n";
    query.append( "<D:searchrequest xmlns:D=\"DAV:\">\r\n" );
    query.append( q.toUtf8() );
    query.append( "</D:searchrequest>\r\n" );

    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return this->createRequest("SEARCH", req, query);
}

QNetworkReply* QWebdav::get(const QString& path)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return QNetworkAccessManager::get(req);
}

QNetworkReply* QWebdav::get(const QString& path, QIODevice* data)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    QNetworkReply* reply = QNetworkAccessManager::get(req);
    m_inDataDevices.insert(reply, data);
    connect(reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));
    return reply;
}

QNetworkReply* QWebdav::put(const QString& path, QIODevice* data)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return QNetworkAccessManager::put(req, data);
}

QNetworkReply* QWebdav::put(const QString& path, const QByteArray& data)
{  
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return QNetworkAccessManager::put(req, data);
}

QNetworkReply* QWebdav::propfind(const QString& path, const QWebdav::PropNames& props, int depth)
{
    QByteArray query;

    query = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    query += "<D:propfind xmlns:D=\"DAV:\" >";
    query += "<D:prop>";
    foreach (QString ns, props.keys())
    {
        foreach (const QString key, props[ns])
            if (ns == "DAV:")
                query += "<D:" + key + "/>";
            else
                query += "<" + key + " xmlns=\"" + ns + "\"/>";
    }
    query += "</D:prop>";
    query += "</D:propfind>";
    return propfind(path, query, depth);
}


QNetworkReply* QWebdav::propfind(const QString& path, const QByteArray& query, int depth)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));
    req.setRawHeader("Depth", depth == 2 ? QString("infinity").toUtf8() : QString::number(depth).toUtf8());

    return createRequest("PROPFIND", req, query);
}

QNetworkReply* QWebdav::proppatch(const QString& path, const QWebdav::PropValues& props)
{
    QByteArray query;

    query = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    query += "<D:proppatch xmlns:D=\"DAV:\" >";
    query += "<D:prop>";
    foreach (QString ns, props.keys())
    {
        QMap < QString , QVariant >::const_iterator i;

        for (i = props[ns].constBegin(); i != props[ns].constEnd(); ++i) {
            if (ns == "DAV:") {
                query += "<D:" + i.key() + ">";
                query += i.value().toString();
                query += "</D:" + i.key() + ">" ;
            } else {
                query += "<" + i.key() + " xmlns=\"" + ns + "\">";
                query += i.value().toString();
                query += "</" + i.key() + " xmlns=\"" + ns + "\"/>";
            }
        }
    }
    query += "</D:prop>";
    query += "</D:propfind>";

    return proppatch(path, query);
}

QNetworkReply* QWebdav::proppatch(const QString& path, const QByteArray& query)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return createRequest("PROPPATCH", req, query);
}

QNetworkReply* QWebdav::mkdir (const QString& path)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return createRequest("MKCOL", req);
}

QNetworkReply* QWebdav::copy(const QString& pathFrom, const QString& pathTo, bool overwrite)
{
    QNetworkRequest req;
    req.setUrl(QUrl(pathFrom));
    req.setRawHeader("Destination", pathTo.toUtf8());
    req.setRawHeader("Depth", "infinity");
    req.setRawHeader("Overwrite", overwrite ? "T" : "F");

    return createRequest("COPY", req);
}

QNetworkReply* QWebdav::move(const QString& pathFrom, const QString& pathTo, bool overwrite)
{
    QNetworkRequest req;
    req.setUrl(QUrl(pathFrom));
    req.setRawHeader("Destination", pathTo.toUtf8());
    req.setRawHeader("Depth", "infinity");
    req.setRawHeader("Overwrite", overwrite ? "T" : "F");

    return createRequest("MOVE", req);
}

QNetworkReply* QWebdav::remove(const QString& path)
{
    QNetworkRequest req;
    req.setUrl(QUrl(path));

    return createRequest("DELETE", req);
}
