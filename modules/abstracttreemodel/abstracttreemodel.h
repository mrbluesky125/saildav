/* This file is part of meedav
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

#ifndef ABSTRACT_TREE_MODEL_H
#define ABSTRACT_TREE_MODEL_H

#include <QtCore>
#include <QtDeclarative>

#include "abstracttreeitem.h"

///\brief Provides a basic tree model that handles the basic tree structure suitable for a QTreeView.
///The standard implementation of data() and setData() uses the property names that are mapped to the given roles.
///In subclasses it is recommended to define this mapping through setRoleNames() in the constructor. This enables
///also access to the properties from QML views.
///
///\remarks This class is basically intended for just 1 column, more columns
///used in QTreeView requires further considerations :-) (e.g. you can set/get
///particular properties for other columns in the data access functions)
///
///\see AbstractTreeItem
///
///\author Timo Zimmermann
class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    AbstractTreeModel(AbstractTreeItem* rootItem = 0);
    virtual ~AbstractTreeModel();

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool insertRow(int row, AbstractTreeItem* item, const QModelIndex& parent = QModelIndex());
    bool insertRows(int row, const QList<AbstractTreeItem*>& items, const QModelIndex& parent = QModelIndex());
    bool moveRow(int from, int to, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int rows, const QModelIndex& parent = QModelIndex());
    AbstractTreeItem* takeRow(int row, const QModelIndex &parent = QModelIndex());

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void sort(const char* propertyName, Qt::SortOrder order = Qt::AscendingOrder, const QModelIndex &index = QModelIndex());

    AbstractTreeItem* getItem(const QModelIndex &index) const;
    QModelIndex getIndex(const AbstractTreeItem* item) const;

protected:
    AbstractTreeItem* m_rootItem;	///<Root item of the model

private:
    Q_DISABLE_COPY(AbstractTreeModel)

};

QML_DECLARE_TYPE(AbstractTreeModel)

#endif
