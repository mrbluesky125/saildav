#include "webdavmodel.h"
#include "webdavfileinfo.h"

QWebdavModel::QWebdavModel(QObject *parent) : AbstractTreeModel(new QWebdavUrlInfo("/"))
  ,m_folder("/")
  ,m_homePath("/")
  ,m_localRootPath(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/MyDocs/meedav/")
  ,m_refreshFlag(false)
{
    QHash<int, QByteArray> roles = roleNames();
    roles[FileNameRole] = "displayName";
    roles[FilePathRole] = "name";
    setRoleNames(roles);

    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
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
    return AbstractTreeModel::data( this->index(index.row(), 0, getIndex(currentItem())), role );
}

QString QWebdavModel::folder() const
{
    return m_folder;
}

void QWebdavModel::setFolder(const QString& path)
{
    if(m_folder == path || !createPath(path)) return;

    m_folder = path;
    qDebug() << "QWebdavModel | Set folder:" << path;
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
    //loadCache(localRootPath());
    cd(homePath());
    //QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
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
    qDebug() << "QWebdavModel | NetworkError:" << error;

    //sender is always a network reply, no check necessary
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    emit errorChanged(reply->errorString());
}

void QWebdavModel::replySslError(const QList<QSslError> &errors)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    reply->ignoreSslErrors();
    qDebug() << "QWebdavModel | Ignored SSL errors: " << errors;
}

void QWebdavModel::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    qDebug() << "QWebdavModel | Authentification for" << baseUrl() << "required.";
    qDebug() << "QWebdavModel | Trying to login...";
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
    if(item == 0 || item == m_rootItem)
        return dynamic_cast<QWebdavUrlInfo*>(m_rootItem)->name();
    else
        return dynamic_cast<QWebdavUrlInfo*>(item->parentItem())->name();
}

bool QWebdavModel::createPath(const QString& path)
{
    if(path == "/")
        return true;

    qDebug() << "QWebdavModel | Create path:" << path;

    if(!path.startsWith('/')) {
        qDebug() << "QWebdavModel | Cannot create a relative path:" << path;
        return false;
    }

    //check for existing item
    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem != m_rootItem) {
        qDebug() << "QWebdavModel | Path found:" << path;
        return true;
    }

    //step through the path parts
    QStringList dirNames = path.split('/', QString::SkipEmptyParts);
    QString currentPath = dynamic_cast<QWebdavUrlInfo*>(m_rootItem)->name();
    foreach(QString dir, dirNames) {
        if(dir == ".");
        else if(dir == "..")
            currentPath = parentFolder(currentPath + "/");
        else
            currentPath = createFolder(currentPath, dir);
    }

    qDebug() << "QWebdavModel | Path created:" << path;
    return true;
}

QString QWebdavModel::createFolder(const QString& path, const QString& dirName)
{
    qDebug() << "QWebdavModel | Create folder:" << path;

    if(path.isEmpty() || dirName.isEmpty())
        return path;

    //search
    foreach(AbstractTreeItem* item, findItem(path)->childList()) {
        QWebdavUrlInfo* urlInfoItem = static_cast<QWebdavUrlInfo*>(item);
        if(urlInfoItem->name().endsWith(dirName + "/")) {
            qDebug() << "QWebdavModel | Folder found:" << dirName << "in path:" << path;
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

    qDebug() << "QWebdavModel | Create local dir:" << urlInfoItem->name();
    if(!QDir().mkpath(localRootPath() + urlInfoItem->name())) {
        qDebug() << "QWebdavModel | Failed to create local dir:" << urlInfoItem->name();
    }

    qDebug() << "QWebdavModel | Folder created:" << dirName << "in path:" << path;
    return urlInfoItem->name();
}

QWebdavUrlInfo* QWebdavModel::findItem(const QString& path) const
{
    QWebdavUrlInfo* item = m_rootItem->findFirst<QWebdavUrlInfo>(path, "name");
    return item == 0 ? dynamic_cast<QWebdavUrlInfo*>(m_rootItem) : item;
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

    qDebug() << "QWebdavModel | Create local dir:" << path;
    if(!QDir().mkpath(path)) {
        qDebug() << "QWebdavModel | Failed to create local dir:" << path;
    }
}

bool QWebdavModel::loadCache(const QString &path)
{
    qDebug() << "QWebdavModel | Trying to load cache file from:" << path;

    QFile cacheFile(path+".xmlCache");
    if(!cacheFile.open(QFile::ReadOnly)) {
        qDebug() << "QWebdavModel | Cache file" << cacheFile.fileName() << "not available";
        return false;
    }

    QXmlStreamReader reader(&cacheFile);
    m_rootItem->removeChildren(0, m_rootItem->childCount());
    reader.readNextStartElement();
    if(m_rootItem->readFromXml(reader)) {
        qDebug() << "QWebdavModel | Cache file succesfully loaded.";
        return true;
    }
    return false;
}

bool QWebdavModel::saveCache(const QString &path)
{
    QFile cacheFile(path+".xmlCache");
    if(!cacheFile.open(QFile::WriteOnly)) {
        qDebug() << "QWebdavModel | Cache file" << cacheFile.fileName() << "not writable.";
        return false;
    }

    QXmlStreamWriter writer(&cacheFile);
    writer.setAutoFormatting(true);
    return m_rootItem->writeToXml(writer);
}

void QWebdavModel::rename(const QString& path, const QString& to)
{
    qDebug() << "QWebdavModel | move:" << path << "to:" << to;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qDebug() << "QWebdavModel | Cannot rename entry, not found:" << currentItem->name();
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
    qDebug() << "QWebdavModel | remove:" << path;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qDebug() << "QWebdavModel | Cannot remove entry, not found:" << currentItem->name();
        return;
    }

    QNetworkReply* reply = m_webdavManager.remove(baseUrl() + path);
    currentItem->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::upload(const QString& path, const QString& from)
{
    qDebug() << "QWebdavModel | upload " << from << "to:" << path;

    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qDebug() << "QWebdavModel | Cannot upload" << from << ", path not found:" << path;
        return;
    }

    QScopedPointer<QFile> file( new QFile(QUrl(from).toLocalFile()) );
    if(!file->open(QFile::ReadOnly)) {
        qDebug() << "QWebdavModel | Failed to open file:" << from;
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
    qDebug() << "QWebdavModel | mkdir:" << path;
    if(!createPath(path)) {
        qDebug() << "QWebdavModel | Failed to create new remote folder:" << path;
        return;
    }

    QNetworkReply* reply = m_webdavManager.mkdir(baseUrl() + path);
    findItem(path)->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::download(const QString& path)
{
    qDebug() << "QWebdavModel | get:" << path;

    //I assume that the file is visible and in the cache
    QWebdavUrlInfo* currentItem = findItem(path);
    if(currentItem == 0) {
        qDebug() << "QWebdavModel | Failed to download file. File not found:" << path;
        return;
    }

    QString localPath = localRootPath() + parentFolder(currentItem->name());
    QScopedPointer<QFile> file(new QFile(localPath + currentItem->displayName()));
    if(!file->open(QFile::WriteOnly)) {
        qDebug() << "QWebdavModel | Unable to write to local file:" << localPath;
        return;
    }

    QNetworkReply* reply = m_webdavManager.get(baseUrl() + path, file.take());
    currentItem->setReply(reply);
    connectReply(reply);
}

void QWebdavModel::cd(const QString& dir)
{
    QString path = rectifyPath(dir);
    qDebug() << "QWebdavModel | cd:" << path;
    setFolder(path);
}

void QWebdavModel::refresh()
{
    qDebug() << "QWebdavModel | Current folder:" << folder();
    qDebug() << "QWebdavModel | List url:" << baseUrl() + folder();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + folder());
    findItem(folder())->setReply(reply);
    connectReply(reply);

    m_refreshFlag = false;
}


