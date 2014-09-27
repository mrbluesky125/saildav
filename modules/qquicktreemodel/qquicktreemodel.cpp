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


#include "qquicktreeitem.h"
#include "qquicktreemodel.h"

QQuickTreeModel::QQuickTreeModel(QQuickTreeItem* rootItem) : QAbstractItemModel()
  ,m_rootItem(rootItem == 0 ? new QQuickTreeItem() : rootItem)
{

}

QQuickTreeModel::~QQuickTreeModel()
{
    delete m_rootItem;
}


///\brief Returns the item at the given model index
///\param index The index of the requested item
///\return The requested item or a the root item if the index is invalid
QQuickTreeItem* QQuickTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QQuickTreeItem* item = reinterpret_cast<QQuickTreeItem*>(index.internalPointer());
        if (item != nullptr) return item;
    }
    return m_rootItem;
}

///\brief Returns the index of the given item
///\param index The index of the requested item
///\return The requested index or an invalid index if the item is not found
QModelIndex QQuickTreeModel::getIndex(const QQuickTreeItem* item) const
{
    if (item != 0 && item->parentItem() != 0) {
        unsigned int row = item->childNumber();
        return createIndex(row, 0, const_cast<QQuickTreeItem*>(item));
    }
    return QModelIndex();
}

void QQuickTreeModel::loadFromJson(const QString &filename)
{
    QFile jsonFile(filename);
    if(!jsonFile.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Unable to open file" << filename << "for reading.";
        return;
    }

    QJsonObject rootObject = QJsonDocument::fromJson(jsonFile.readAll()).object();
    m_rootItem->readFromJson(rootObject);
    jsonFile.close();
}

void QQuickTreeModel::saveToJson(const QString &filename) const
{
    QFile jsonFile(filename);
    if(!jsonFile.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << "Unable to open file" << filename << "for writing.";
        return;
    }

    QJsonObject rootObject;
    m_rootItem->writeToJson(rootObject);
    jsonFile.write(QJsonDocument(rootObject).toJson());
    jsonFile.close();
}

///\brief returns an index at a specific row and column of a parent index
///\param row The row number of the item
///\param column The column number of the item (currently ignored)
///\param parent The parent index
///\remarks The column is currently ignored
///\return The requested index
QModelIndex QQuickTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    //Is parent a valid model index?
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    //Retrieve the item pointer
    QQuickTreeItem* parentItem = getItem(parent);

    QQuickTreeItem* childItem = parentItem->child(row);
    if (childItem != 0)
        return createIndex(row, column, static_cast<void*>(childItem));
    else
        return QModelIndex();
}

///\brief Inserts a item at the given row of a given parent index
///\param row The row at wich the item is inserted
///\param item The item that is inserted
///\param parent The parent index in at which the item is inserted
///return returns true if the item was inserted, otherwise false
bool QQuickTreeModel::insertRow(int row, QQuickTreeItem* item, const QModelIndex& parent)
{
    QQuickTreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, row, row);
    success = parentItem->insertChild(row, item);
    endInsertRows();

    return success;
}

///\brief Inserts a list of items at the given row of a given parent index
///\param row The row at wich the items are inserted
///\param items The item list that is inserted
///\param parent The parent index in at which the items are inserted
///return returns true if the items were inserted, otherwise false
bool QQuickTreeModel::insertRows(int row, const QList<QQuickTreeItem*>& items, const QModelIndex& parent)
{
    QQuickTreeItem* parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, row, row + items.count() - 1);
    success = parentItem->insertChildren(row, items);
    endInsertRows();

    return success;
}

///\brief Moves the item at row 'from' of item 'parent' to row 'to' of item 'parent'.
///\return True if the row was successfully moved
bool QQuickTreeModel::moveRow(int from, int to, const QModelIndex& parent)
{
    QQuickTreeItem* parentItem = getItem(parent);

    if(from < 0 || to < 0 || from > parentItem->childCount() || to > parentItem->childCount())
        return false;
    //TODO: beginMoveRows()
    QQuickTreeItem* item = takeRow(from, parent);
    insertRow(to, item, parent);

    return true;
}

///\brief Returns the parent index of a given index
///\return The parent index of the given index
QModelIndex QQuickTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QQuickTreeItem* childItem = getItem(index);
    QQuickTreeItem* parentItem = childItem->parentItem();

    if (parentItem == 0 || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

///\brief Removes a row from a given parent index
///\param row The specific row that is removed
///\param parent The parent index from wich the row is removed
///\return true if the row could be removed, otherwise false
bool QQuickTreeModel::removeRow(int row, const QModelIndex &parent)
{
    QQuickTreeItem* parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, row, row);
    success = parentItem->removeChildren(row, 1);
    endRemoveRows();

    return success;
}

///\brief Removes multiple row from a given parent index
///\param row The specific row from which the romoving starts
///\param rows The number of rows that are removed
///\param parent The parent index from wich the row is removed
///\return true if the rows could be removed, otherwise false
bool QQuickTreeModel::removeRows(int row, int rows, const QModelIndex &parent)
{
    QQuickTreeItem* parentItem = getItem(parent);
    if(parentItem == 0)
        return false;

    beginRemoveRows(parent, row, row + rows - 1);
    bool success = parentItem->removeChildren(row, rows);
    endRemoveRows();

    return success;
}

///\brief Returns the total number of rows of the given parent index
///\param parent The index on which the counting is performed
///\return The row count
int QQuickTreeModel::rowCount(const QModelIndex &parent) const
{
    QQuickTreeItem* parentItem = getItem(parent);
    if(parentItem == 0)
        return -1;

    return parentItem->childCount();
}

///\brief Takes the row at the position 'row' from the given parent
///\param parent The parent index from wich the row is taken
///\param row The row that is removed
///\return The removed item at the row
QQuickTreeItem* QQuickTreeModel::takeRow(int row, const QModelIndex &parent)
{
    QQuickTreeItem* parentItem = getItem(parent);

    beginRemoveRows(parent, row, row);
    QQuickTreeItem* item = parentItem->takeChild(row);
    endRemoveRows();

    return item;
}

///\brief Standard implementation
int QQuickTreeModel::columnCount(const QModelIndex& parent) const
{
    return m_rootItem->columnCount();
}

///\brief Standard implementation.
///Treats the role names as property names.
QVariant QQuickTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QQuickTreeItem* item = reinterpret_cast<QQuickTreeItem*>(index.internalPointer());
    QHash<int, QByteArray> roleNames = this->roleNames();

    if(!roleNames.contains(role))
        return QVariant();

    return item->property(roleNames.value(role));
}

///\brief Standard implementation
///Treats the role names as property names.
bool QQuickTreeModel::setData(const QModelIndex & index, const QVariant & value, int role/*=Qt::EditRole*/)
{
    if(!index.isValid())
        return false;

    QQuickTreeItem *item = static_cast<QQuickTreeItem*>(index.internalPointer());
    QHash<int, QByteArray> roleNames = this->roleNames();

    if(!roleNames.contains(role))
        return false;
    
    return item->setProperty(roleNames.value(role), value);
}

///\brief Standard implementation
Qt::ItemFlags QQuickTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    QQuickTreeItem *item = static_cast<QQuickTreeItem*>(index.internalPointer());
    return item->flags();
}

///\brief Sorts the model by column in the given order.
///\param column The column that is used for sorting
///\param order Sorting order (ascending/descending)
void QQuickTreeModel::sort(int column, Qt::SortOrder order)
{
    sort("title", order);
}

///\brief Sorts the model by column in the given order.
///\param propertyName The column that is used for sorting
///\param order Sorting order (ascending/descending)
void QQuickTreeModel::sort(const char* propertyName, Qt::SortOrder order, const QModelIndex &index)
{
    if(QString(propertyName).isEmpty())
        return;

    QQuickTreeItem* item = getItem(index);

    //bubblesort child list
    for(int n=item->childCount(); n>1; n--) {
        for(int i=0; i<n-1; i++) {
            if(item->lessThan(item->child(i)->property(propertyName), item->child(i+1)->property(propertyName)) != (order == Qt::DescendingOrder)) {
                moveRow(i, i+1, getIndex(item));
            }
        }
    }

    //tell all childs to sort their child list
    for(int i = 0; i<item->childCount(); i++)
        sort(propertyName, order, getIndex(item->child(i)));
}
