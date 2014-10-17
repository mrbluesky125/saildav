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

#include <QtDebug>

#include "qquicktreeitem.h"

QMap<QString, std::function<QQuickTreeItem*()> > QQuickTreeItem::s_createFunctions;

QQuickTreeItem::QQuickTreeItem(QObject* parent) : QObject(parent)
  ,m_parentItem( qobject_cast<QQuickTreeItem*>(parent) )
{
    //insert self to parents child list - only if parent is also a QQuickTreeItem
    if(m_parentItem != nullptr)
        m_parentItem->insertChild(m_parentItem->childCount(), this);
}

///\brief Destruktor. Deletes all childs.
QQuickTreeItem::~QQuickTreeItem()
{
    qDeleteAll(m_childItems);
}

///\brief returns the item at the given index
///\param number The index of the requested item
///\return The item at the given index
QQuickTreeItem *QQuickTreeItem::child(int number) const
{
    return m_childItems.value(number, 0);
}

///\brief returns the child list as qml compatible list property
QQmlListProperty<QQuickTreeItem> QQuickTreeItem::childs()
{
    return QQmlListProperty<QQuickTreeItem>(this, 0, &QQuickTreeItem::append, &QQuickTreeItem::count, &QQuickTreeItem::at, &QQuickTreeItem::clear);
}

///\brief returns the child list
QList<QQuickTreeItem*> QQuickTreeItem::childList() const
{
    return m_childItems;
}

///\brief Total number of child items
///\return Number of child items
int QQuickTreeItem::childCount() const
{
    return m_childItems.count();
}

///\brief Returns the index of the item in its parent child list
///\return Index of the item in its parent child list
int QQuickTreeItem::childNumber() const
{
    if (m_parentItem != nullptr)
        return m_parentItem->m_childItems.indexOf(const_cast<QQuickTreeItem*>(this));

    return 0;
}

///\brief returns the total number of children (complete tree branch)
///\return total number of items in this branch of the tree
int QQuickTreeItem::itemCount() const
{
    unsigned int itemCount = childCount();
    foreach(QQuickTreeItem* item, m_childItems) {
        itemCount += item->itemCount();
    }
    return itemCount;
}

///\brief Appends a item to the child list
///\param The item that is appended
///\return true if the item was successfully added
bool QQuickTreeItem::addChild(QQuickTreeItem* item)
{
    return insertChild(m_childItems.size(), item);
}

///\brief Inserts a item at a given position
///\param position The position where the new item is inserted
///\param item The item that is inserted
///\return true if the item was successfully inserted
bool QQuickTreeItem::insertChild(int position, QQuickTreeItem* item)
{
    if(item == 0 || position < 0 || position > m_childItems.size()) {
        qCWarning(MODULES_QQUICKTREEMODEL) << QObject::tr("New child item could not be inserted.") << item;
        return false;
    }

    if(m_childItems.contains(item)) {
        qCWarning(MODULES_QQUICKTREEMODEL) << QObject::tr("Item is already in the list") << item;
        return false;
    }

    item->setParentItem(this);
    m_childItems.insert(position, item);

    emit childsChanged();
    return true;
}

///\brief Inserts multiple items at a given position 
///\param position The position where the new items are inserted
///\param items The items that are inserted to the list
///\return true if the items was successfully inserted
bool QQuickTreeItem::insertChildren(int position, const QList<QQuickTreeItem*>& items)
{
    if (position < 0 || position > m_childItems.size()) {
        qCWarning(MODULES_QQUICKTREEMODEL) << QObject::tr("New child item could not be inserted.");
        return false;
    }

    foreach(QQuickTreeItem* item, items) {
        item->setParentItem(this);
        m_childItems.insert(position, item);
    }

    emit childsChanged();
    return true;
}

///\brief returns the parent item of the item
QQuickTreeItem *QQuickTreeItem::parentItem() const
{
    return m_parentItem;
}

///\brief sets the parent of this item to the given item
///\param parent the new parent of the item
///\remarks the item is NOT added to the parents child list
void QQuickTreeItem::setParentItem(QQuickTreeItem* parent)
{
    //remove the item from the current parent
    if(m_parentItem != 0)
        m_parentItem->removeChild(this);

    m_parentItem = parent;
    emit parentItemChanged();
}

///\brief Returns the indent of the item, which is also the nesting depth of the item in the tree
unsigned int QQuickTreeItem::indent() const
{
    return m_parentItem == 0 ? 0 : m_parentItem->indent() + 1;
}

///\brief Moves the item at row from to row to.
///\return True if the row was successfully moved
bool QQuickTreeItem::moveChild(int from, int to)
{
    if(from < 0 || to < 0 || from > m_childItems.size() || to > m_childItems.size())
        return false;

    m_childItems.move(from, to);
    emit childsChanged();
    return true;
}

///\brief Removes the given item from the child list
///\param item the item that is removed from the children list
///\remarks the removed item is DELETED
bool QQuickTreeItem::removeChild(QQuickTreeItem* item)
{
    int position = m_childItems.indexOf(item);

    if(position == -1)
        return false;

    //remove and set parent to NULL
    QQuickTreeItem* removedChild = m_childItems.takeAt(position);
    emit childsChanged();

    delete removedChild;
    return true;
}

///\brief Removes multiple items from the list at a specific position
///\param position index from where the items are deleted
///\param count The number of items that are deleted
///\remarks The removed items are DELETED
bool QQuickTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    QList<QQuickTreeItem*> removedChilds;
    for (int row = 0; row < count; ++row)
        removedChilds << m_childItems.takeAt(position);

    emit childsChanged();

    qDeleteAll(removedChilds);
    return true;
}

///\brief Takes the item at the given position from the child list and returns it.
///\param position of the item that is taken from the child list
///\remarks the taken item is NOT deleted / the caller has to delete it
QQuickTreeItem* QQuickTreeItem::takeChild(int position)
{
    if (position < 0 || position >= m_childItems.size())
        return 0;

    QQuickTreeItem* item = m_childItems.takeAt(position);
    emit childsChanged();

    return item;
}

///\brief Comparsion function used for sorting
///Has to be reimplemented if custom QVariant types are used.
bool QQuickTreeItem::lessThan(const QVariant& left, const QVariant& right)
{
    if(left.type() == QVariant::Int)
        return left.toInt() < right.toInt();
    else if(left.type() == QVariant::UInt)
        return left.toUInt() < right.toUInt();
    else if(left.type() == QVariant::LongLong)
        return left.toLongLong() < right.toLongLong();
    else if(left.type() == QVariant::Double)
        return left.toDouble() < right.toDouble();
    else if(left.type() == QVariant::Char)
        return left.toChar() < right.toChar();
    else if(left.type() == QVariant::Date)
        return left.toDate() < right.toDate();
    else if(left.type() == QVariant::Time)
        return left.toTime() < right.toTime();
    else if(left.type() == QVariant::DateTime)
        return left.toDateTime() < right.toDateTime();
    else if(left.type() == QVariant::String)
        return left.toString() < right.toString();
    else
        return false;
}

void QQuickTreeItem::fromJson(QJsonObject json)
{
    removeChildren(0, childCount());

    QJsonArray jsonChildList = json["childs"].toArray();
    foreach(QJsonValue jsonChild, jsonChildList) {
        QJsonObject childObject = jsonChild.toObject();
        QQuickTreeItem* newChild = s_createFunctions.value(childObject.value("itemType").toString(), [this]()->QQuickTreeItem*{ return new QQuickTreeItem(); })();
        newChild->fromJson(childObject);
        addChild(newChild);
    }
}

QJsonObject QQuickTreeItem::toJson() const
{
    QJsonObject json;
    json["itemType"] = "QQuickTreeItem";

    if(childCount() > 0) {
        QJsonArray jsonChildList;
        foreach(QQuickTreeItem* child, childList()) {
            jsonChildList.append(child->toJson());
        }
        json["childs"] = jsonChildList;
    }

    return json;
}

///----------------------------------------------------------QML-List Interface-----------------------------------------------------------------//

template<typename ElementType>
QQuickTreeItem* listObject(QQmlListProperty<ElementType>* list)
{
    return qobject_cast<QQuickTreeItem*>(list->object);
}

void QQuickTreeItem::append(QQmlListProperty<QQuickTreeItem> *list, QQuickTreeItem *item)
{
    if(listObject(list) == nullptr) return;
    listObject(list)->addChild(item);
}

int QQuickTreeItem::count(QQmlListProperty<QQuickTreeItem> *list)
{
    if(listObject(list) == nullptr) return 0;
    return listObject(list)->childCount();
}

QQuickTreeItem * QQuickTreeItem::at(QQmlListProperty<QQuickTreeItem> *list, int index)
{
    if(listObject(list) == nullptr) return nullptr;
    return listObject(list)->child(index);
}

void QQuickTreeItem::clear(QQmlListProperty<QQuickTreeItem>* list)
{
    if(listObject(list) == nullptr) return;
    listObject(list)->removeChildren(0, listObject(list)->childCount());
}
