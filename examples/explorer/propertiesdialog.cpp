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

#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#include "propertiesdialog.h"

PropertiesDialog::PropertiesDialog(const QWebdavUrlInfo & infos, QWidget * parent)
  : QDialog(parent)
{
  QStyle *style = QApplication::style();
  QIcon icon;

  setupUi(this);

  if (infos.isDir()) {
    icon = style->standardIcon(QStyle::SP_DirIcon);
    nameLabel->setText(QFileInfo(infos.name()).dir().dirName());
    typeLabel->setText(tr("Folder"));
  } else {
    icon = style->standardIcon(QStyle::SP_FileIcon);
    nameLabel->setText(QFileInfo(infos.name()).fileName());
    typeLabel->setText(infos.mimeType());
  }

  sizeLabel->setText(QString("%1").arg(infos.size()));
  createdAtLabel->setText(infos.createdAt().toString());
  lastModifiedLabel->setText(infos.lastModified().toString());

  iconLabel->setPixmap(icon.pixmap(64, 64));

  QString str;
  QTextStream text(&str);

  text << infos.propElement();
  textEdit->setText(str);
}

PropertiesDialog::~PropertiesDialog()
{
}

