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

#ifndef APP_H
# define APP_H

#include <QWebdav>
#include <QWebdavUrlInfo>
#include <QtNetwork/QAuthenticator>

#include <QLineEdit>
#include <QPushButton>
#include <QIcon>
#include <QProgressBar>
#include <QFile>
#include <QProgressDialog>
#include <QUrl>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

public slots:
  void loadUrl(const QString & url);

private slots:
  void authenticationRequired(const QString &hostname, quint16 port,
			      QAuthenticator *authenticator);
  void stateChanged(int state);
  void responseHeaderReceived( const QHttpResponseHeader & resp );
  void dataProgress(int done, int total);
  void done(bool error);
  void requestFinished(int id, bool error);

  void listInfo(const QWebdavUrlInfo & url);
  void loadUrl();
  void itemDoubleClicked(QTreeWidgetItem * item, int column);
  void updateActions();

  void download(QTreeWidgetItem *item, const QString & url, bool open = false);

  void up();
  void open();
  void download();
  void upload();
  void infos();
  void refresh();
  void mkdir();
  void unlink();

private:
  void createActions();
  void createToolbar();
  void createStatusbar();
  void createWebdav();
  void createTreeWidget();
  void createProgressDialog();

  void cleanup();

  void insertParentDirectory(const QString & url);
  QString itemToUrl(QTreeWidgetItem * item, bool *isDir = NULL);

  QLineEdit *urlLineEdit;
  QPushButton *urlPushButton;
  QWebdav *webdav;

  QProgressBar *progress;

  QString host;
  quint64 port;
  QString user;

  QIcon dirIcon;
  QIcon fileIcon;
  QIcon parentIcon;

  QProgressDialog *progressDialog;
  QMap < int , QFile * > uploadFiles;
  QMap < int , QFile * > downloadFiles;
  QMap < int , bool > openDownload;
  QMap < QTreeWidgetItem *, QWebdavUrlInfo * > urlInfos;

  bool needRefresh;
  QUrl currentUrl;
};

#endif
