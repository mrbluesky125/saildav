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

///\brief Destruktor. Deletes all childs.
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
QList<AbstractTreeItem*> AbstractTreeItem::childList() const
{
    return m_childItems;
}

///\brief Total number of child items
///\return Number of child items
int AbstractTreeItem::childCount() const 
{
    return m_childItems.count();
}

///\brief Returns the index of the item in its parent child list
///\return Index of the item in its parent child list
int AbstractTreeItem::childNumber() const 
{
    if (m_parentItem != 0)
        return m_parentItem->m_childItems.indexOf(const_cast<AbstractTreeItem*>(this));

    return 0;
}

///\brief returns the total number of children (complete tree branch)
///\return total number of items in this branch of the tree
int AbstractTreeItem::itemCount() const 
{
    unsigned int itemCount = childCount();
    foreach(AbstractTreeItem* item, m_childItems) {
        itemCount += item->itemCount();
    }
    return itemCount;
}

///\brief Appends a item to the child list
///\param The item that is appended
///\return true if the item was successfully added
bool AbstractTreeItem::addChild(AbstractTreeItem* item) 
{
    return insertChild(m_childItems.size(), item);
}

///\brief Inserts a item at a given position
///\param position The position where the new item is inserted
///\param item The item that is inserted
///\return true if the item was successfully inserted
bool AbstractTreeItem::insertChild(int position, AbstractTreeItem* item) 
{
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
bool AbstractTreeItem::insertChildren(int position, const QList<AbstractTreeItem*>& items) 
{
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
AbstractTreeItem *AbstractTreeItem::parentItem() const 
{
    return m_parentItem;
}

///\brief sets the parent of this item to the given item
///\param parent the new parent of the item
///\remarks the item is NOT added to the parents child list
void AbstractTreeItem::setParentItem(AbstractTreeItem* parent) 
{
    //remove the item from the current parent
    if(m_parentItem != 0)
        m_parentItem->removeChild(this);

    m_parentItem = parent;
    emit parentItemChanged();
}

///\brief Returns the indent of the item, which is also the nesting depth of the item in the tree
unsigned int AbstractTreeItem::indent() const 
{
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
bool AbstractTreeItem::removeChild(AbstractTreeItem* item) 
{
    int position = m_childItems.indexOf(item);

    if(position == -1)
        return false;

    //remove and set parent to NULL
    delete m_childItems.takeAt(position);

    emit childsChanged();
    return true;
}

///\brief Removes multiple items from the list at a specific position
///\param position index from where the items are deleted
///\param count The number of items that are deleted
///\remarks The removed items are DELETED
bool AbstractTreeItem::removeChildren(int position, int count) 
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    emit childsChanged();
    return true;
}

///\brief Takes the item at the given position from the child list and returns it.
///\param position of the item that is taken from the child list
///\remarks the taken item is NOT deleted / the caller has to delete it
AbstractTreeItem* AbstractTreeItem::takeChild(int position)
{
    if (position < 0 || position >= m_childItems.size())
        return 0;

    AbstractTreeItem* item = m_childItems.takeAt(position);
    emit childsChanged();

    return item;
}

///\brief Reads the item from the given stream reader.
///\param reader The xml stream reader
///\remarks The stream reader has to be open and the current element of it has to be the item start tag
bool AbstractTreeItem::readFromXml(QXmlStreamReader &reader)
{
    //check if reader is at a start token
    if(!reader.isStartElement() || reader.name() != "item") {
        reader.raiseError("Reader is at a wrong token: " + reader.tokenString());
        return false;
    }

    //read all child xml tags until the end token of the current node is reached
    while(reader.readNextStartElement()) {
        readXmlTags(reader);
    }

    //check for errors
    if(reader.error() != QXmlStreamReader::NoError) {
        qWarning() << "AbstractTreeItem |" << QObject::tr("Parsing of the item failed. Error:") << reader.errorString();
        return false;
    }

    return true;
}

///\brief Writes the item to the given stream writer
///\param reader The xml stream writer
bool AbstractTreeItem::writeToXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement("item");
    writeItemAttributes(writer);
    writeXmlTags(writer);
    writer.writeEndElement();
    return true;
}

///\brief Comparsion function used for sorting
///Has to be reimplemented if custom QVariant types are used.
bool AbstractTreeItem::lessThan(const QVariant& left, const QVariant& right)
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

///\brief Reads the child tags of the item
///\remarks Derived classes has to call this function in the else-branch of their if-else tree
///\note Note that derived classes has to call the implementation of their base class, in order to write all tags, which isn't necessarily this class
void AbstractTreeItem::readXmlTags(QXmlStreamReader &reader)
{
    //create a base type item if the 'item' tag was not handled by derived class
    if( reader.name() == "item" ) {
        QScopedPointer<AbstractTreeItem> item(new AbstractTreeItem());
        if(item->readFromXml(reader)) addChild(item.take());
    }
    else {
        qWarning() << "AbstractTreeItem | Unknown XML element skipped:" << reader.name();
        reader.skipCurrentElement();
    }
}

///\brief Writes the child tags of the item
///\remarks Derived classes has to call this function in their reimplemented version of this function
void AbstractTreeItem::writeXmlTags(QXmlStreamWriter &writer) const
{
    foreach(AbstractTreeItem* item, childList())  {
        item->writeToXml(writer);
    }
}


///\brief Writes attributes of the item. This function is called right after the creation of the item root tag.
///Reimplement this function to write your own item attributes
void AbstractTreeItem::writeItemAttributes(QXmlStreamWriter& writer) const
{
    writer.writeAttribute("objectName", objectName());
}

///----------------------------------------------------------QML-List Interface-----------------------------------------------------------------//

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
