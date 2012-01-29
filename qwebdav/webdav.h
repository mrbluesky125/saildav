/* This file is part of QWebdav
 *
 * Copyright (C) 2009-2010 Corentin Chary <corentin.chary@gmail.com>
 * Copyright (C) 2012 Timo Zimmermann <meedav@timozimmermann.de>
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

#ifndef QWEBDAV_H
#define QWEBDAV_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlInfo>

class QWebdavUrlInfo;

/**
 * @brief Main class used to handle the webdav protocol
 */
class QWebdav : public QNetworkAccessManager
{
    Q_OBJECT

private:
    QMap<QNetworkReply*, QIODevice*> m_outDataDevices;
    QMap<QNetworkReply*, QIODevice*> m_inDataDevices;

public:
    QWebdav(QObject* parent = 0);
    ~QWebdav();

    typedef QMap<QString, QMap < QString, QVariant > > PropValues;
    typedef QMap<QString, QStringList > PropNames;

    QNetworkReply* list(const QString& path);
    QNetworkReply* search(const QString& path, const QString& query);
    QNetworkReply* get(const QString& path);
    QNetworkReply* get(const QString& path, QIODevice* data);
    QNetworkReply* put(const QString& path, QIODevice* data);
    QNetworkReply* put(const QString& path, const QByteArray& data );

    QNetworkReply* mkdir(const QString& dir );
    QNetworkReply* copy(const QString& pathFrom, const QString& pathTo, bool overwrite = false);
    QNetworkReply* move(const QString& pathFrom, const QString& pathTo, bool overwrite = false);
    QNetworkReply* remove(const QString& path );

    QNetworkReply* propfind(const QString& path, const QByteArray& query, int depth = 0);
    QNetworkReply* propfind(const QString& path, const QWebdav::PropNames& props, int depth = 0);

    QNetworkReply* proppatch(const QString& path, const QWebdav::PropValues& props);
    QNetworkReply* proppatch(const QString& path, const QByteArray& query);

    /* TODO lock, unlock */
protected slots:
    void replyReadyRead();
    void replyFinished(QNetworkReply*);

protected:
    QNetworkReply* createRequest(const QString& method, QNetworkRequest& req, QIODevice* outgoingData = 0 );
    QNetworkReply* createRequest(const QString& method, QNetworkRequest& req, const QByteArray& outgoingData);
};

#endif // QWEBDAV_H
