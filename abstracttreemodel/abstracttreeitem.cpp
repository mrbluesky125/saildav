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

#include "abstracttreeitem.h"

template<typename ElementType>
AbstractTreeItem* listObject(QDeclarativeListProperty<ElementType>* list)
{
    return qobject_cast<AbstractTreeItem*>(list->object);
}

AbstractTreeItem::AbstractTreeItem(AbstractTreeItem* parent) : QObject(parent)
    ,m_parentItem (parent)
{
    
    //insert self to parent item
    if(m_parentItem != 0)
        m_parentItem->insertChild(m_parentItem->childCount(), this);
}

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(m_childItems);
}

///\brief returns the item at the given index
///\param number The index of the requested item
///\return The item at the given index
AbstractTreeItem *AbstractTreeItem::child(int number) const
{
    return m_childItems.value(number, 0);
}

///\brief returns the child list as qml compatible list property
QDeclarativeListProperty<AbstractTreeItem> AbstractTreeItem::childs()
{
    return QDeclarativeListProperty<AbstractTreeItem>(this, 0, &AbstractTreeItem::append, &AbstractTreeItem::count, &AbstractTreeItem::at);
}

///\brief returns the child list
QList<AbstractTreeItem*> AbstractTreeItem::childList()
{
    return m_childItems;
}

///\brief Total number of child items
///\return Number of child items
unsigned int AbstractTreeItem::childCount() const {
    return m_childItems.count();
}

///\brief Returns the index of the item in its parent child list
///\return Index of the item in its parent child list
unsigned int AbstractTreeItem::childNumber() const {
    if (m_parentItem != 0)
        return m_parentItem->m_childItems.indexOf(const_cast<AbstractTreeItem*>(this));

    return 0;
}

///\brief returns the total number of children (complete tree branch)
///\return total number of items in this branch of the tree
unsigned int AbstractTreeItem::itemCount() const {
    unsigned int itemCount = childCount();
    foreach(AbstractTreeItem* item, m_childItems) {
        itemCount += item->itemCount();
    }
    return itemCount;
}

///\brief Appends a item to the child list
///\param The item that is appended
///\return true if the item was successfully added
bool AbstractTreeItem::addChild(AbstractTreeItem* item) {
    return insertChild(m_childItems.size(), item);
}

///\brief Inserts a item at a given position
///\param position The position where the new item is inserted
///\param item The item that is inserted
///\return true if the item was successfully inserted
bool AbstractTreeItem::insertChild(int position, AbstractTreeItem* item) {
    if(item == 0 || position < 0 || position > m_childItems.size()) {
        qWarning() << "AbstractTreeItem |" << QObject::tr("New child item could not be inserted.") << item;
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
bool AbstractTreeItem::insertChildren(int position, const QList<AbstractTreeItem*>& items) {
    if (position < 0 || position > m_childItems.size()) {
        qWarning() << "AbstractTreeItem |" << QObject::tr("New child item could not be inserted.");
        return false;
    }

    foreach(AbstractTreeItem* item, items) {
        item->setParentItem(this);
        m_childItems.insert(position, item);
    }

    emit childsChanged();
    return true;
}

///\brief returns the parent item of the item
AbstractTreeItem *AbstractTreeItem::parentItem() const {
    return m_parentItem;
}

///\brief sets the parent of this item to the given item
///\param parent the new parent of the item
///\remarks the item is NOT added to the parents child list
void AbstractTreeItem::setParentItem(AbstractTreeItem* parent) {
    //remove the item from the current parent
    if(m_parentItem != 0)
        m_parentItem->removeChild(this);

    m_parentItem = parent;
    emit parentItemChanged(this->parentItem());
    emit indentChanged(indent());
}

///\brief Returns the indent of the item, which is also the nesting depth of the item in the tree
unsigned int AbstractTreeItem::indent() const {
    return m_parentItem == 0 ? 0 : m_parentItem->indent() + 1;
}

///\brief Moves the item at row from to row to.
///\return True if the row was successfully moved
bool AbstractTreeItem::moveChild(int from, int to)
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
bool AbstractTreeItem::removeChild(AbstractTreeItem* item) {
    int position = m_childItems.indexOf(item);

    if(position == -1)
        return false;

    //remove and set parent to NULL
    //m_childItems.takeAt(itemPosition)->setParentItem(NULL);
    delete m_childItems.takeAt(position);

    emit childsChanged();
    return true;
}

///\brief Removes multiple items from the list at a specific position
///\param position index from where the items are deleted
///\param count The number of items that are deleted
///\remarks The removed items are DELETED
bool AbstractTreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        //m_childItems.takeAt(position)->setParentItem(NULL);
        delete m_childItems.takeAt(position);

    emit childsChanged();
    return true;
}

///\brief Searches the item and its childs for the first item that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\return The first item that matches the criterion or 0 if no item matches
AbstractTreeItem* AbstractTreeItem::findFirst(QVariant criterion, const char* propertyName) const {
    //return self if the criterion matches
    if(property(propertyName) == criterion)
        return const_cast<AbstractTreeItem*>(this);

    //search the childs for the item
    for(unsigned int i = 0; i<childCount(); i++) {
        AbstractTreeItem* childItem = child(i)->findFirst(criterion, propertyName);
        if(childItem != 0)
            return childItem;	//return the item if found in the child
    }

    return 0; //return 0 if the item was not found in any childs
}

///\brief Searches the item and its childs for items that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\return A list of items that matches the criterion or an empty list if no item matches
QList<AbstractTreeItem*> AbstractTreeItem::findAll(QVariant criterion, const char* propertyName) const
{
    //create list
    QList<AbstractTreeItem*> items;

    //return self if the criterion matches
    if(property(propertyName) == criterion)
        items.append(const_cast<AbstractTreeItem*>(this));

    //search the childs for the items and append them to the local list
    for(unsigned int i = 0; i<childCount(); i++) {
        items.append( child(i)->findAll(criterion, propertyName) );
    }

    return items; //return the list
}

///\brief Returns all items.
///\return A list of all items.
QList<AbstractTreeItem*> AbstractTreeItem::findAll() const
{
    //create list
    QList<AbstractTreeItem*> items;

    items.append(const_cast<AbstractTreeItem*>(this));
    foreach(AbstractTreeItem* child, m_childItems) {
        items.append(child->findAll());
    }
    return items;
}


void AbstractTreeItem::append(QDeclarativeListProperty<AbstractTreeItem> *list, AbstractTreeItem *item)
{
    if(listObject(list) == 0) return;
    listObject(list)->addChild(item);
}

int AbstractTreeItem::count(QDeclarativeListProperty<AbstractTreeItem> *list)
{
    if(listObject(list) == 0) return 0;
    return listObject(list)->childCount();
}

AbstractTreeItem * AbstractTreeItem::at(QDeclarativeListProperty<AbstractTreeItem> *list, int index)
{
    if(listObject(list) == 0) return 0;
    return listObject(list)->child(index);
}
