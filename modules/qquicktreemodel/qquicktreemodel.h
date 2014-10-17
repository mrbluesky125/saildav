/*
 * Copyright (C) 2012 Timo Zimmermann <zimmermann.emb@googlemail.com>
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

#ifndef Q_QUICK_TREE_MODEL_H
#define Q_QUICK_TREE_MODEL_H

#include <QtCore>
#include <QtQuick>

#include "qquicktreeitem.h"

Q_DECLARE_LOGGING_CATEGORY(MODULES_QQUICKTREEMODEL)

///\brief Provides a basic tree model that handles the basic tree structure suitable for a QTreeView.
///The standard implementation of data() and setData() uses the property names that are mapped to the given roles.
///In subclasses it is recommended to define this mapping through setRoleNames() in the constructor. This enables
///also access to the properties from QML views.
///
///\remarks This class is basically intended for the use in a qml context. There are two possible use cases:
///1. Subclass the model and use it as normal list model for QML views (access the data through the role names)
///2. Subclass the model and use the 'childs' list property of the items (subclass the item and provide Q_PROPERTYs for data access) as model,
///which enables the use of a tree structure.
///
///\see QQuickTreeItem
///
///\author Timo Zimmermann
class QQuickTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QQuickTreeItem* rootItem READ rootItem WRITE setRootItem NOTIFY rootItemChanged)

    Q_CLASSINFO("DefaultProperty", "rootItem")

public:
    QQuickTreeModel(QObject* rootItem = 0);
    virtual ~QQuickTreeModel();

    QQuickTreeItem* rootItem() const;
    void setRootItem(QQuickTreeItem* arg);

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool insertRow(int row, QQuickTreeItem* item, const QModelIndex& parent = QModelIndex());
    bool insertRows(int row, const QList<QQuickTreeItem*>& items, const QModelIndex& parent = QModelIndex());
    bool moveRow(int from, int to, const QModelIndex& parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int rows, const QModelIndex& parent = QModelIndex());
    QQuickTreeItem* takeRow(int row, const QModelIndex &parent = QModelIndex());

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void sort(const char* propertyName, Qt::SortOrder order = Qt::AscendingOrder, const QModelIndex &index = QModelIndex());

    QQuickTreeItem* getItem(const QModelIndex &index = QModelIndex()) const;
    QModelIndex getIndex(const QQuickTreeItem* item) const;

    Q_INVOKABLE void loadFromJson(const QString& filename);
    Q_INVOKABLE void saveToJson(const QString& filename) const;

signals:
    void childsChanged();
    void rootItemChanged();

protected:
    QScopedPointer<QQuickTreeItem> m_rootItem;	///<Root item of the model

};

#endif
