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

#include "webdavmodel.h"
#include "webdavfileinfo.h"

QWebdavModel::QWebdavModel(QObject *parent) : QQuickTreeModel(new QWebdavUrlInfo("/"))
  ,m_folder("/")
  ,m_homePath("/")
  ,m_localRootPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/saildav/")
  ,m_refreshFlag(false)
{
    QObject::connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
}

QWebdavModel::~QWebdavModel()
{
    saveCache(localRootPath());
}

int QWebdavModel::rowCount(const QModelIndex &parent) const
{
    return currentItem()->childCount();
}

QVariant QWebdavModel::data(const QModelIndex &index, int role) const
{
    return QQuickTreeModel::data( this->index(index.row(), 0, getIndex(currentItem())), role );
}

QString QWebdavModel::folder() const
{
    return m_folder;
}

void QWebdavModel::setFolder(const QString& path)
{
    if(m_folder == path) return;

    if(!createPath(path)) {
        qCWarning(Q_WEBDAV) << "Creating folder failed." << path;
        return;
    }

    m_folder = path;
    qCDebug(Q_WEBDAV) << "Set folder:" << path;
    QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
    emit folderChanged();
}

QString QWebdavModel::parentFolder() const
{
    return parentFolder(folder());
}

QWebdavUrlInfo* QWebdavModel::currentItem() const
{
    return findItem(folder()) ;
}

QString QWebdavModel::baseUrl() const
{
    return m_baseUrl.toString(QUrl::RemovePath | QUrl::RemoveUserInfo);
}

QString QWebdavModel::userName() const
{
    return m_baseUrl.userName();
}

QString QWebdavModel::password() const
{
    return m_baseUrl.password();
}

QString QWebdavModel::homePath() const
{
    return m_baseUrl.path().isEmpty() ? "/" : m_baseUrl.path();
}

void QWebdavModel::setBaseUrl(const QString& baseUrl)
{
    if(m_baseUrl.toString() == baseUrl) return;

    QString userName = this->userName();
    QString password = this->password();

    m_baseUrl.setUrl(baseUrl);
    m_baseUrl.setUserName(userName);
    m_baseUrl.setPassword(password);
    emit baseUrlChanged();

    setLocalRootPath(localRootPath()+m_baseUrl.host()+"/");
}

void QWebdavModel::setUserName(const QString& userName)
{
    if(m_baseUrl.userName() == userName) return;

    m_baseUrl.setUserName(userName);
    emit userNameChanged();
}

void QWebdavModel::setPassword(const QString& password)
{
    if(m_baseUrl.password() == password) return;

    m_baseUrl.setPassword(password);
    emit passwordChanged();
}

void QWebdavModel::classBegin()
{
    setLocalRootPath(m_localRootPath); //make sure the local application dir is created
}

void QWebdavModel::componentComplete()
{
    loadCache(localRootPath());
    cd(homePath());
    QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
}

void QWebdavModel::replyFinished()
{
    if(m_refreshFlag == true)
        QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);

    beginResetModel();
    endResetModel();
}

void QWebdavModel::replyError(QNetworkReply::NetworkError error)
{
    qCWarning(Q_WEBDAV) << "NetworkError:" << error;

    //sender is always a network reply, no check necessary
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    emit errorChanged(reply->errorString());
}

void QWebdavModel::replySslError(const QList<QSslError> &errors)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    reply->ignoreSslErrors();
    qCDebug(Q_WEBDAV) << "Ignored SSL errors: " << errors;
}

void QWebdavModel::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    qCDebug(Q_WEBDAV) << "Authentification for" << baseUrl() << "required.";
    qCDebug(Q_WEBDAV) << "Trying to login...";
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}

void QWebdavModel::connectReply(QNetworkReply* reply)
{
    if(reply == 0) return;
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(replySslError(const QList<QSslError> &)));
}

QString QWebdavModel::parentFolder(const QString& path) const
{
    QWebdavUrlInfo* item = findItem(path);
    if(item == 0 || item == m_rootItem.data())
        return dynamic_cast<QWebdavUrlInfo*>(m_rootItem.data())->name();
    else
        return dynamic_cast<QWebdavUrlInfo*>(item->parentItem())->name();
}

bool QWebdavModel::createPath(const QString& path)
{
    if(path == "/")
        return true;

    qCDebug(Q_WEBDAV) << "Create path:" << path;

    if(!path.startsWith('/')) {
        qCDebug(Q_WEBDAV) << "Cannot create a relative path:" << path;
        return false;
    }

    //check for existing item
    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem != m_rootItem.data()) {
        qCDebug(Q_WEBDAV) << "Path found:" << path;
        return true;
    }

    //step through the path parts
    QStringList dirNames = path.split('/', QString::SkipEmptyParts);
    QString currentPath = dynamic_cast<QWebdavUrlInfo*>(m_rootItem.data())->name();
    foreach(QString dir, dirNames) {
        if(dir == ".");
        else if(dir == "..")
            currentPath = parentFolder(currentPath + "/");
        else
            currentPath = createFolder(currentPath, dir);
    }

    qCDebug(Q_WEBDAV) << "Path created:" << path;
    return true;
}

QString QWebdavModel::createFolder(const QString& path, const QString& dirName)
{
    qCDebug(Q_WEBDAV) << "Create folder:" << path;

    if(path.isEmpty() || dirName.isEmpty())
        return path;

    //search
    foreach(QQuickTreeItem* item, findItem(path)->childList()) {
        QWebdavUrlInfo* urlInfoItem = static_cast<QWebdavUrlInfo*>(item);
        if(urlInfoItem->name().endsWith(dirName + "/")) {
            qCDebug(Q_WEBDAV) << "Folder found:" << dirName << "in path:" << path;
            return urlInfoItem->name();
        }
    }

    //create
    QWebdavUrlInfo* urlInfoItem = new QWebdavUrlInfo(path + dirName + "/");
    urlInfoItem->setDisplayName(dirName);
    urlInfoItem->setDir(true);
    urlInfoItem->setFile(false);
    urlInfoItem->setLastModified(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"));

    QModelIndex parentIndex = getIndex(findItem(path));
    insertRow(0, urlInfoItem, parentIndex);

    qCDebug(Q_WEBDAV) << "Create local dir:" << urlInfoItem->name();
    if(!QDir().mkpath(localRootPath() + urlInfoItem->name())) {
        qCWarning(Q_WEBDAV) << "Failed to create local dir:" << urlInfoItem->name();
    }

    qCDebug(Q_WEBDAV) << "Folder created:" << dirName << "in path:" << path;
    return urlInfoItem->name();
}

QWebdavUrlInfo* QWebdavModel::findItem(const QString& path) const
{
    QWebdavUrlInfo* item = m_rootItem->findFirst<QWebdavUrlInfo>(path, "name");
    return item == 0 ? dynamic_cast<QWebdavUrlInfo*>(m_rootItem.data()) : item;
}

QString QWebdavModel::rectifyPath(const QString& path)
{
    QString rectified = path;
    rectified = rectified.startsWith("/") ? rectified : folder() + rectified;
    rectified = rectified.endsWith("/") ? rectified : rectified + "/";
    return rectified;
}

QString QWebdavModel::localRootPath() const
{
    return m_localRootPath;
}

void QWebdavModel::setLocalRootPath(const QString& path)
{
    m_localRootPath = path;

    qCDebug(Q_WEBDAV) << "Create local dir:" << path;
    if(!QDir().mkpath(path)) {
        qCWarning(Q_WEBDAV) << "Failed to create local dir:" << path;
    }
}

bool QWebdavModel::loadCache(const QString &path)
{
    qCDebug(Q_WEBDAV) << "Trying to load cache file from:" << path;

    QFile cacheFile(path + ".jsonCache");
    if(!cacheFile.open(QFile::ReadOnly)) {
        qCWarning(Q_WEBDAV) << "Cache file" << cacheFile.fileName() << "not available";
        return false;
    }

    QJsonObject cacheObject = QJsonDocument::fromJson(cacheFile.readAll()).object();
    if(cacheObject.isEmpty()) {
        qCWarning(Q_WEBDAV) << "Cache file empty or parser error occured.";
        return false;
    }

    //qCDebug(Q_WEBDAV) << "Cache file:\n" << QJsonDocument(cacheObject).toJson();

    m_rootItem->removeChildren(0, m_rootItem->childCount());
    m_rootItem->fromJson(cacheObject);

    qCDebug(Q_WEBDAV) << "Cache file succesfully loaded.";
    return true;
}

bool QWebdavModel::saveCache(const QString &path)
{
    QFile cacheFile(path + ".jsonCache");
    if(!cacheFile.open(QFile::WriteOnly)) {
        qDebug() << "QWebdavModel | Cache file" << cacheFile.fileName() << "not writable.";
        return false;
    }

    QJsonObject cacheObject = m_rootItem->toJson();
    cacheFile.write(QJsonDocument(cacheObject).toJson(QJsonDocument::Indented));

    //qCDebug(Q_WEBDAV) << "Cache file:\n" << QJsonDocument(cacheObject).toJson();

    return true;
}

void QWebdavModel::rename(const QString& path, const QString& to)
{
    qCDebug(Q_WEBDAV) << "move:" << path << "to:" << to;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qCWarning(Q_WEBDAV) << "Cannot rename entry, not found:" << currentItem->name();
        return;
    }

    //map relative 'to' path to the folder of the 'from' item
    QString pathTo = to;
    if(!to.startsWith('/'))
        pathTo = static_cast<QWebdavUrlInfo*>(currentItem->parentItem())->name() + to;

    QNetworkReply* reply = m_webdavManager.move(baseUrl() + path, baseUrl() + pathTo);
    currentItem->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::remove(const QString& path)
{
    qCDebug(Q_WEBDAV) << "remove:" << path;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qCWarning(Q_WEBDAV) << "Cannot remove entry, not found:" << currentItem->name();
        return;
    }

    QNetworkReply* reply = m_webdavManager.remove(baseUrl() + path);
    currentItem->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::upload(const QString& path, const QString& from)
{
    qCDebug(Q_WEBDAV) << "upload " << from << "to:" << path;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qCWarning(Q_WEBDAV) << "Cannot upload" << from << ", path not found:" << path;
        return;
    }

    QScopedPointer<QFile> file( new QFile(QUrl(from).toLocalFile()) );
    if(!file->open(QFile::ReadOnly)) {
        qCWarning(Q_WEBDAV) << "Failed to open file:" << from;
        return;
    }

    QFileInfo toFileInfo = QFileInfo(file->fileName());
    QString toFilePath = path + toFileInfo.fileName();

    QWebdavUrlInfo* urlInfoItem = new QWebdavUrlInfo();
    urlInfoItem->setName(toFilePath);
    urlInfoItem->setDisplayName(toFileInfo.fileName());
    urlInfoItem->setDir(false);
    urlInfoItem->setFile(true);
    urlInfoItem->setSize(toFileInfo.size());
    urlInfoItem->setLastModified(toFileInfo.lastModified().toString("dd.MM.yyyy hh:mm"));
    currentItem->addChild(urlInfoItem);

    QNetworkReply* reply = m_webdavManager.put(baseUrl() + toFilePath, file.take());
    urlInfoItem->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::mkdir(const QString& path)
{
    qCDebug(Q_WEBDAV) << "mkdir:" << path;
    if(!createPath(path)) {
        qCWarning(Q_WEBDAV) << "Failed to create new remote folder:" << path;
        return;
    }

    QNetworkReply* reply = m_webdavManager.mkdir(baseUrl() + path);
    findItem(path)->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::download(const QString& path)
{
    qCDebug(Q_WEBDAV) << "get:" << path;

    //I assume that the file is visible and in the cache
    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qCWarning(Q_WEBDAV) << "Failed to download file. File not found:" << path;
        return;
    }

    QString localPath = localRootPath() + parentFolder(currentItem->name());
    QScopedPointer<QFile> file(new QFile(localPath + currentItem->displayName()));
    if(!file->open(QFile::WriteOnly)) {
        qCWarning(Q_WEBDAV) << "Unable to write to local file:" << localPath;
        return;
    }

    QNetworkReply* reply = m_webdavManager.get(baseUrl() + path, file.take());
    currentItem->setReply(reply);
    connectReply(reply);
}

void QWebdavModel::cd(const QString& dir)
{
    QString path = rectifyPath(dir);
    qCDebug(Q_WEBDAV) << "cd:" << path;
    setFolder(path);
}

void QWebdavModel::refresh()
{
    qCDebug(Q_WEBDAV) << "Current folder:" << folder();
    qCDebug(Q_WEBDAV) << "List url:" << baseUrl() + folder();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + folder());
    findItem(folder())->setReply(reply);
    connectReply(reply);

    m_refreshFlag = false;
}


