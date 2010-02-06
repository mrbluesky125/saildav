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

#include <QDebug>
#include <QToolBar>
#include <QUrl>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QProgressBar>
#include <QInputDialog>
#include <QFile>
#include <QIODevice>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QDesktopServices>

#include "mainwindow.h"
#include "propertiesdialog.h"

MainWindow::MainWindow()
{
  setupUi(this);

  createActions();
  createToolbar();
  createStatusbar();
  createWebdav();
  createTreeWidget();
  createProgressDialog();

  needRefresh = false;
}

MainWindow::~MainWindow()
{
  /* Close all files an remove temporary files */
  foreach (QFile *file, downloadFiles.values())
    delete file;
}

void
MainWindow::createActions()
{
  QStyle *style = QApplication::style();

  actionDownload->setIcon(style->standardIcon(QStyle::SP_DialogSaveButton));
  actionOpen->setIcon(style->standardIcon(QStyle::SP_DialogOpenButton));
  actionUpload->setIcon(style->standardIcon(QStyle::SP_DriveNetIcon));
  actionDelete->setIcon(style->standardIcon(QStyle::SP_TrashIcon));
  actionRefresh->setIcon(style->standardIcon(QStyle::SP_BrowserReload));
  actionCreateDir->setIcon(style->standardIcon(QStyle::SP_FileDialogNewFolder));
  actionGo->setIcon(style->standardIcon(QStyle::SP_DialogApplyButton));
  actionParent->setIcon(style->standardIcon(QStyle::SP_FileDialogToParent));
  actionInformations->setIcon(style->standardIcon(QStyle::SP_FileDialogInfoView));

  dirIcon = style->standardIcon(QStyle::SP_DirIcon);
  fileIcon = style->standardIcon(QStyle::SP_FileIcon);
  parentIcon = style->standardIcon(QStyle::SP_DirOpenIcon);

  connect(actionRefresh, SIGNAL(triggered()), this, SLOT(refresh()));
  connect(actionParent, SIGNAL(triggered()), this, SLOT(up()));
  connect(actionDownload, SIGNAL(triggered()), this, SLOT(download()));
  connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
  connect(actionUpload, SIGNAL(triggered()), this, SLOT(upload()));
  connect(actionDelete, SIGNAL(triggered()), this, SLOT(unlink()));
  connect(actionInformations, SIGNAL(triggered()), this, SLOT(infos()));
  connect(actionCreateDir, SIGNAL(triggered()), this, SLOT(mkdir()));
  connect(actionGo, SIGNAL(triggered()), this, SLOT(loadUrl()));
}

void
MainWindow::createStatusbar()
{
  progress = new QProgressBar(this);

  statusBar()->insertPermanentWidget(0, progress);
}

void
MainWindow::createToolbar()
{
  QToolBar *tb;

  urlLineEdit = new QLineEdit(this);

  tb = addToolBar(tr("Actions"));
  tb->addAction(actionParent);
  tb->addAction(actionRefresh);
  tb->addSeparator();
  tb->addAction(actionDownload);
  tb->addAction(actionOpen);
  tb->addAction(actionUpload);
  tb->addSeparator();
  tb->addAction(actionInformations);
  tb->addAction(actionDelete);
  tb->addAction(actionCreateDir);
  tb->addSeparator();
  tb->addWidget(urlLineEdit);
  tb->addAction(actionGo);
  addToolBar(Qt::TopToolBarArea, tb);

  connect(urlLineEdit, SIGNAL(returnPressed()), this, SLOT(loadUrl()));
}

void
MainWindow::createWebdav()
{
  webdav = new QWebdav(this);
  port = 80;
  host = "";
  connect(webdav, SIGNAL(sslErrors( const QList<QSslError> &)),
	  webdav, SLOT(ignoreSslErrors()));
  connect(webdav, SIGNAL(authenticationRequired(const QString &, quint16,
						QAuthenticator *)),
	  this, SLOT(authenticationRequired(const QString &, quint16,
					    QAuthenticator *)));
  connect(webdav, SIGNAL(listInfo(const QWebdavUrlInfo &)),
	  this, SLOT(listInfo(const QWebdavUrlInfo &)));
  connect(webdav, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
  connect(webdav, SIGNAL(responseHeaderReceived( const QHttpResponseHeader & )),
	  this, SLOT(responseHeaderReceived( const QHttpResponseHeader & )));
  connect(webdav, SIGNAL(dataReadProgress ( int, int )),
	  this, SLOT(dataProgress(int, int)));
  connect(webdav, SIGNAL(dataSendProgress ( int, int )),
	  this, SLOT(dataProgress(int, int)));
  connect(webdav, SIGNAL(done(bool)), this, SLOT(done(bool)));
  connect(webdav, SIGNAL(requestFinished(int, bool)),
	  this, SLOT(requestFinished(int, bool)));
}

void
MainWindow::createTreeWidget()
{
  treeWidget->header()->setStretchLastSection(false);
  treeWidget->header()->setResizeMode(0,QHeaderView::Stretch);

  connect(treeWidget, SIGNAL(itemDoubleClicked( QTreeWidgetItem *, int )),
	  this, SLOT(itemDoubleClicked( QTreeWidgetItem *, int )));
  connect(treeWidget, SIGNAL(itemSelectionChanged ()),
	  this, SLOT(updateActions()));

  treeWidget->addAction(actionOpen);
  treeWidget->addAction(actionDownload);
  treeWidget->addAction(actionUpload);
  treeWidget->addAction(actionDelete);
  treeWidget->addAction(actionCreateDir);
  treeWidget->addAction(actionInformations);
}

void
MainWindow::createProgressDialog()
{
  progressDialog = new QProgressDialog(this);

  connect(progressDialog, SIGNAL(canceled()), webdav, SLOT(abort()));
}

void
MainWindow::itemDoubleClicked(QTreeWidgetItem * item, int column)
{
  bool dir;
  QWebdavUrlInfo *infos = urlInfos.value(item);

  /* Must be ".." */
  if (!infos)
    up();
  else {
    if (infos->isDir())
      loadUrl(currentUrl.toString() + QFileInfo(infos->name()).dir().dirName());
    else
      download(item, currentUrl.toString() + QFileInfo(infos->name()).fileName(), true);
  }
}

void
MainWindow::dataProgress(int done, int total)
{
  progress->setMaximum(total);
  progress->setValue(done);
  progressDialog->setMaximum(total);
  progressDialog->setValue(done);
}

void
MainWindow::stateChanged(int state)
{
  QString text = "Unknown";

  if (state == QHttp::Unconnected)
    text = "Unconnected";
  if (state == QHttp::HostLookup)
    text = "HostLookup";
  if (state == QHttp::Connecting)
    text = "Connecting";
  if (state == QHttp::Sending)
    text = "Sending";
  if (state == QHttp::Reading)
    text = "Reading";
  if (state == QHttp::Connected)
    text = "Connected";
  if (state == QHttp::Closing)
    text = "Closing";
  statusBar()->showMessage(text, 10000);
}

void
MainWindow::responseHeaderReceived( const QHttpResponseHeader & resp )
{
  int code = resp.statusCode();
  QString msg;

  if (code < 400)
    return ;
  msg = QString("%1: %2").arg(resp.statusCode()).arg(resp.reasonPhrase());
  if (code >= 500)
    QMessageBox::critical(this, tr("Server error"), msg);
  else
    QMessageBox::warning(this, tr("Client error"), msg);
  cleanup();
}

void
MainWindow::cleanup()
{
  treeWidget->clear();
}

void
MainWindow::requestFinished(int id, bool error)
{
  dataProgress(1, 1);
  progressDialog->hide();

  if (uploadFiles[id])
    delete uploadFiles.take(id);

  if (downloadFiles[id]) {
    QFile *file = downloadFiles[id];
    bool open = openDownload.take(id);

    file->close();
    if (open) {
      QDesktopServices::openUrl("file://" + file->fileName());
    } else { /* Don't delete temporary files */
      downloadFiles.remove(id);
      delete file;
    }
  }

  if (needRefresh) {
    needRefresh = false;
    refresh();
  }
}

void
MainWindow::done ( bool error )
{
  if (error) {
    QMessageBox::warning(this, tr("Webdav error"), webdav->errorString());
    cleanup();
  }
}

void
MainWindow::listInfo(const QWebdavUrlInfo & url)
{
  QStringList strings;

  if (QUrl(url.name()).path() == currentUrl.path())
    return ;

  if (url.isDir())
    strings.append(QFileInfo(url.name()).dir().dirName());
  else
    strings.append(QFileInfo(url.name()).fileName());

  if (!url.isDir())
    strings.append(QString("%1").arg(url.size()));
  else
    strings.append(QString());

  if (!url.isDir())
    strings.append(url.mimeType());
  else
    strings.append(tr("directory"));

  if (!url.lastModified().isNull())
    strings.append(url.lastModified().toString());
  else
    strings.append(url.createdAt().toString());

  QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget, strings);
  item->setIcon(0, url.isDir() ? dirIcon : fileIcon);
  treeWidget->insertTopLevelItem(1, item);

  urlInfos.insert(item, new QWebdavUrlInfo(url));
}

void
MainWindow::authenticationRequired(const QString &hostname, quint16 port,
				   QAuthenticator *authenticator)
{
  bool ok;
  QUrl url(urlLineEdit->text());
  QString user;
  QString pass;

  user = url.userName();
  pass = url.password();
  if (user.isEmpty())
    user = QInputDialog::getText(this, tr("Authentication"),
				 tr("User name:"), QLineEdit::Normal,
				 QDir::home().dirName(), &ok);
  if (pass.isEmpty())
    pass = QInputDialog::getText(this, tr("Authentication"),
				 tr("Password:"), QLineEdit::Password,
				 tr(""), &ok);
  authenticator->setUser(user);
  authenticator->setPassword(pass);
}

void
MainWindow::loadUrl(const QString & str)
{
  QUrl url;
  QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;

  if (!str.startsWith("http://") && !str.startsWith("https://")) {
    QMessageBox::critical(this, tr("Error"),
			  tr("Invalid protocol, must be http:// or https://"));
  }
  if (!str.endsWith("/"))
    url.setUrl(str + "/");
  else
    url.setUrl(str);

  if (!url.isValid()) {
    QMessageBox::critical(this, tr("Error"), tr("Invalid url"));
    return ;
  }
  if (url.port() == -1) {
    if (str.startsWith("https://"))
      url.setPort(443);
    if (str.startsWith("http://"))
      url.setPort(80);
  }

  if (str.startsWith("https://"))
    mode = QHttp::ConnectionModeHttps;
  else
    mode = QHttp::ConnectionModeHttp;

  if (host != url.host() || port != url.port()) {
    host = url.host();
    port = url.port();
    webdav->setHost(host, mode, port);
  }

  foreach (QTreeWidgetItem *item, urlInfos.keys())
    delete urlInfos.take(item);
  treeWidget->clear();

  insertParentDirectory(url.toString());
  webdav->list(url.encodedPath());
  urlLineEdit->setText(url.toString());
  currentUrl = url;
}

void
MainWindow::up(void)
{
  QString url = currentUrl.toString();

  url.truncate(url.size() - 1);
  url = QFileInfo(url).dir().path();
  url += "/";
  loadUrl(url);
}

void
MainWindow::download(QTreeWidgetItem *item, const QString & str, bool open)
{
  QUrl url(str);
  qint64 size = item->data(1, Qt::DisplayRole).toUInt();
  QString type = item->data(2, Qt::DisplayRole).toString();
  QString fileName;
  QFile *file;

  fileName = QFileInfo(str).fileName();

  if (open)
    file = new QTemporaryFile(QDir::tempPath() + "/XXXXXX." + fileName);
  else {
    fileName = QFileDialog::getSaveFileName(this, tr("Where do you want to download that file ?"),
					    QDir::currentPath() + "/" + fileName);
    if (fileName.isNull())
      return ;

    file = new QFile(fileName);
  }

  if (!file->open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Download error"),
			  tr("Can't open file '%1' for writing.").arg(fileName));
    delete file;
    return;
  }


  int id = webdav->get(url.encodedPath(), file);

  downloadFiles[id] = file;
  openDownload[id] = open;

  progressDialog->setLabelText(tr("Downloading %1...").arg(str));
  progressDialog->show();
}


void
MainWindow::updateActions()
{
  bool dir = false;
  bool file = false;

  foreach (QTreeWidgetItem *item, treeWidget->selectedItems ()) {
    QString name = item->data(1, Qt::DisplayRole).toString();
    QString type = item->data(2, Qt::DisplayRole).toString();

    if (name == "..")
      continue;
    if (type == "directory")
      dir = true;
    else
      file = true;
    if (file == true)
      break ;
  }

  actionDownload->setEnabled(file);
  actionOpen->setEnabled(file);
  actionDelete->setEnabled(file || dir);
}

void
MainWindow::open()
{
  foreach (QTreeWidgetItem *item, treeWidget->selectedItems ()) {
    QWebdavUrlInfo *url = urlInfos.value(item);

    if (url || url->isDir())
      download(item, url->name(), true);
  }
}

void
MainWindow::download()
{
  foreach (QTreeWidgetItem *item, treeWidget->selectedItems ()) {
    QWebdavUrlInfo *url = urlInfos.value(item);

    if (url || url->isDir())
      download(item, url->name(), false);
  }
}

void
MainWindow::upload()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("What do you want to upload ?"));

  if (fileName.isNull())
    return ;

  QFile *file = new QFile(fileName);

  if (!file->open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, tr("Upload error"),
			  tr("Can't open file '%1' for reading.").arg(fileName));
    delete file;
    return;
  }

  QString path = currentUrl.encodedPath();

  path += QUrl::toPercentEncoding(QFileInfo(fileName).fileName());
  qDebug() << path;
  int id = webdav->put(path, file);

  uploadFiles[id] = file;
  needRefresh = true;
}

void
MainWindow::infos()
{
  foreach (QTreeWidgetItem *item, treeWidget->selectedItems ()) {
    QWebdavUrlInfo *fileInfo = urlInfos.value(item);

    if (!fileInfo)
      continue ;

    PropertiesDialog dlg(*fileInfo, this);
    dlg.exec();
  }
}

void
MainWindow::mkdir()
{
  QString dir = QInputDialog::getText(this, tr("Create Folder"), tr("Folder name"));

  if (!dir.isNull()) {
     QString path = currentUrl.encodedPath();

     path += QUrl::toPercentEncoding(dir);
     webdav->mkdir(path);
  }
  needRefresh = true;
}

void
MainWindow::unlink()
{
  foreach (QTreeWidgetItem *item, treeWidget->selectedItems ()) {
    QWebdavUrlInfo *infos = urlInfos.value(item);

    if (!infos)
      continue ;

    QString path = currentUrl.encodedPath();

    if (infos->isDir())
      path += QUrl::toPercentEncoding(QFileInfo(infos->name()).dir().dirName());
    else
      path += QUrl::toPercentEncoding(QFileInfo(infos->name()).fileName());
    webdav->remove(path);
  }
  needRefresh = true;
}

void
MainWindow::refresh()
{
  loadUrl(currentUrl.toString());
}

void
MainWindow::loadUrl()
{
  QString str = urlLineEdit->text();

  loadUrl(str);
}

void
MainWindow::insertParentDirectory(const QString & url)
{
  QStringList strings;

  strings.append("..");
  strings.append(QString());
  strings.append(tr("directory"));
  strings.append(QString());

  QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget, strings);
  item->setIcon(0, parentIcon);
  treeWidget->insertTopLevelItem(0, item);
  treeWidget->sortItems(0, Qt::AscendingOrder);
}
