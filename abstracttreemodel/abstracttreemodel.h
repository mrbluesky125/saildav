/* This file is part of Meedav
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

///\brief Provides a basic tree model that handles AbstractTreeItems
class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    typedef AbstractTreeItem ItemType;			///<The item type of the model
    typedef QSharedPointer<ItemType> ItemPointer;	///<Smart pointer type for the items

    AbstractTreeModel(ItemPointer rootItem);
    AbstractTreeModel(const AbstractTreeModel&);
    virtual ~AbstractTreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool insertRow(int row, ItemType* item, const QModelIndex& parent = QModelIndex());
    bool insertRows(int row, const QList<ItemType*>& items, const QModelIndex& parent = QModelIndex());
    bool moveRow(int from, int to, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int rows, const QModelIndex& parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    ItemType* getItem(const QModelIndex &index) const;
    QModelIndex getIndex(const ItemType* item) const;

protected:
    ItemPointer m_rootItem;	///<Root item of the model
};

#endif
