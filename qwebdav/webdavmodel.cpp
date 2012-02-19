#include "webdavmodel.h"
#include "webdavfileinfo.h"

QWebdavModel::QWebdavModel(QObject *parent) : AbstractTreeModel(new QWebdavUrlInfo("/"))
  ,m_folder("/")
  ,m_homePath("/")
  ,m_userName("")
  ,m_password("")
  ,m_refreshFlag(false)
{
    connect(&m_webdavManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
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
    //if(m_folder == path) return;

    if(!createPath(path)) return;

    m_folder = path;
    QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
    emit folderChanged();
}

QString QWebdavModel::parentFolder() const
{
    return parentFolder(folder());
}

QWebdavUrlInfo* QWebdavModel::currentItem() const
{
    return item(folder()) ;
}

QString QWebdavModel::baseUrl() const
{
    return m_baseUrl.toString(QUrl::RemovePath);
}

QString QWebdavModel::userName() const
{
    return m_userName;
}

QString QWebdavModel::password() const
{
    return m_password;
}

QString QWebdavModel::homePath() const
{
    return m_homePath;
}

void QWebdavModel::setBaseUrl(const QString& baseUrl)
{
    //if(m_baseUrl.toString() == baseUrl) return;

    m_baseUrl.setUrl(baseUrl);
    emit baseUrlChanged(baseUrl);

    setHomePath(m_baseUrl.path());
    cd(homePath());
}

void QWebdavModel::setUserName(const QString& userName)
{
    if(m_userName == userName) return;

    m_userName = userName;
    emit userNameChanged(m_userName);
}

void QWebdavModel::setPassword(const QString& password)
{
    if(m_password == password) return;

    m_password = password;
    emit passwordChanged(m_password);
}

void QWebdavModel::setHomePath(const QString& homePath)
{
    if(m_homePath == homePath) return;

    m_homePath = homePath;
    emit homePathChanged(m_homePath);
}

void QWebdavModel::classBegin()
{

}

void QWebdavModel::componentComplete()
{
    cd(homePath());
}

void QWebdavModel::replyFinished()
{
    if(m_refreshFlag == true)
        QMetaObject::invokeMethod(this, "refresh", Qt::QueuedConnection);
}

void QWebdavModel::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "QWebdavModel | NetworkError:" << error;
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply == 0) return;
    emit errorChanged(reply->errorString());
}

void QWebdavModel::authenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator)
{
    qDebug() << "QWebdavModel | Authentification for" << m_baseUrl.toString() << "required";
    qDebug() << "QWebdavModel | Login with user:" << userName();
    authenticator->setUser(userName());
    authenticator->setPassword(password());
}

void QWebdavModel::connectReply(QNetworkReply* reply)
{
    if(reply == 0) return;
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
}

QString QWebdavModel::parentFolder(const QString& path) const
{
    QWebdavUrlInfo* item = this->item(path);
    if(item == 0 || item == m_rootItem)
        return dynamic_cast<QWebdavUrlInfo*>(m_rootItem)->name();
    else
        return dynamic_cast<QWebdavUrlInfo*>(item->parentItem())->name();
}

bool QWebdavModel::createPath(const QString& path)
{
    qDebug() << "QWebdavModel | Create path:" << path;

    if(!path.startsWith('/')) {
        qDebug() << "QWebdavModel | Cannot create a relative path:" << path;
        return false;
    }

    //check for existing item
    QWebdavUrlInfo* currentItem = item(path);
    if(currentItem != 0)
        return true;

    //step through the path parts
    QStringList dirNames = path.split('/', QString::SkipEmptyParts);
    QString currentPath = "/";
    foreach(QString dir, dirNames) {
        if(dir == ".");
        else if(dir == "..")
            currentPath = parentFolder(currentPath + "/");
        else
            currentPath = createFolder(currentPath, dir);
    }
    return true;
}

QString QWebdavModel::createFolder(const QString& path, const QString& dirName)
{
    if(path.isEmpty() || dirName.isEmpty()) return path;

    //search
    foreach(AbstractTreeItem* item, item(path)->childList()) {
        QWebdavUrlInfo* urlInfoItem = static_cast<QWebdavUrlInfo*>(item);
        if(urlInfoItem->name().endsWith(dirName + "/"))
            return urlInfoItem->name();
    }

    //create
    QWebdavUrlInfo* urlInfoItem = new QWebdavUrlInfo(path + dirName + "/");
    urlInfoItem->setDisplayName(dirName);
    urlInfoItem->setDir(true);
    urlInfoItem->setFile(false);
    urlInfoItem->setLastModified(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm"));
    item(path)->addChild(urlInfoItem);

    qDebug() << "QWebdavModel | Create folder:" << dirName;

    return urlInfoItem->name();
}

 QWebdavUrlInfo* QWebdavModel::item(const QString& path) const
 {
     return m_rootItem->findFirst<QWebdavUrlInfo>(path, "name");
 }

void QWebdavModel::rename(const QString& path, const QString& to)
{
    qDebug() << "QWebdavModel | move:" << path << "to:" << to;

    QWebdavUrlInfo* currentItem = item(path);
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

    QWebdavUrlInfo* currentItem = item(path);
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

    QWebdavUrlInfo* currentItem = item(path);
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
        qDebug() << "QWebdavModel | Failed to create new folder:" << path;
        return;
    }

    QNetworkReply* reply = m_webdavManager.mkdir(baseUrl() + path);
    item(path)->setReply(reply);
    connectReply(reply);
    m_refreshFlag = true;
}

void QWebdavModel::download(const QString& path)
{
    qDebug() << "QWebdavModel | get:" << path;

    //I assume that the file is visible and in the cache
    QWebdavUrlInfo* currentItem = item(path);
    if(currentItem == 0) {
        qDebug() << "QWebdavModel | Failed to download file. File not found:" << path;
        return;
    }

    QString localPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/" + currentItem->displayName();
    QScopedPointer<QFile> file(new QFile(localPath));
    if(!file->open(QFile::WriteOnly)) {
        qDebug() << "QWebdavModel | Unable to write to local file:" << localPath;
        return;
    }

    currentItem->setDownloadPath(localPath);

    QNetworkReply* reply = m_webdavManager.get(baseUrl() + path, file.take());
    currentItem->setReply(reply);
    connectReply(reply);
}

void QWebdavModel::cd(const QString& dir)
{
    qDebug() << "QWebdavModel | cd:" << dir;
    QString path = dir.startsWith("/") ? dir : folder() + dir;
    if(!path.endsWith('/')) path += "/";
    setFolder(path);
}

void QWebdavModel::refresh()
{
    qDebug() << "QWebdavModel | List url:" << baseUrl() + folder();
    QNetworkReply* reply = m_webdavManager.list(baseUrl() + folder());
    m_refreshFlag = false;
    currentItem()->setReply(reply);
    connectReply(reply);
}


