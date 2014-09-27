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

#ifndef ABSTRACT_TREE_ITEM_H
#define ABSTRACT_TREE_ITEM_H

#include <QtQuick>

#include <QtCore>
#include <QtDebug>

class AbstractTreeItem;

template<typename T> inline T* findItem(const AbstractTreeItem* item, QVariant criterion, const char* propertyName);
template<typename T> inline QList<T*> findItems(const AbstractTreeItem* item, QVariant criterion, const char* propertyName);

///\brief Provides a tree item that handles a basic tree structure for the tree model and is QML aware
///With a QML view you can work directly on the properties of the item and don't have to worry about any data/setData functions.
///You can also use the 'childs' property to display the childs in a QML list view.
class AbstractTreeItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(AbstractTreeItem* parentItem READ parentItem WRITE setParentItem NOTIFY parentItemChanged)
    Q_PROPERTY(QDeclarativeListProperty<AbstractTreeItem> childs READ childs NOTIFY childsChanged)
    Q_PROPERTY(unsigned int indent READ indent NOTIFY parentItemChanged)

    friend class AbstractTreeModel;

public:
    AbstractTreeItem(AbstractTreeItem *parentItem = 0);
    virtual ~AbstractTreeItem();

    AbstractTreeItem* child(int number) const;
    QList<AbstractTreeItem*> childList() const;

    QDeclarativeListProperty<AbstractTreeItem> childs();
    static void append(QDeclarativeListProperty<AbstractTreeItem>* list, AbstractTreeItem* item);
    static int count(QDeclarativeListProperty<AbstractTreeItem>* list);
    static AbstractTreeItem* at(QDeclarativeListProperty<AbstractTreeItem>* list, int index);
    
    AbstractTreeItem* parentItem() const;
    void setParentItem(AbstractTreeItem* parentItem);

    unsigned int indent() const;

    int childCount() const;
    int childNumber() const;
    int itemCount() const;
    bool addChild(AbstractTreeItem* item);
    bool insertChild(int position, AbstractTreeItem* item);
    bool insertChildren(int position, const QList<AbstractTreeItem*>& items);
    bool moveChild(int from, int to);
    bool removeChild(AbstractTreeItem* item);
    bool removeChildren(int position, int count);
    AbstractTreeItem* takeChild(int position);
    
    virtual Qt::ItemFlags flags() const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable; }
    virtual int columnCount() const { return 1; }

    bool readFromXml(QXmlStreamReader& reader);
    bool writeToXml(QXmlStreamWriter& writer) const;

    ///\brief Returns the first item in the node that matches the criterion of the given property and that can be cast to type T
    ///\tparam T The type of the item. Could be the type of a derived class
    ///\returns Returns a pointer to the found item or 0 if nothing was found
    template<typename T> inline T* findFirst(QVariant criterion, const char* propertyName) const
    { return findItem<T>(this, criterion, propertyName, -1); }

    ///\brief Returns the first child that matches the criterion of the given property and that can be cast to type T
    ///\tparam T The type of the item. Could be the type of a derived class
    ///\returns Returns a pointer to the found item or 0 if nothing was found
    template<typename T> inline T* findFirstChild(QVariant criterion, const char* propertyName) const
    { return findItem<T>(this, criterion, propertyName, 1); }

    ///\brief Returns all items in the node that matches the criterion of the given property and that can be cast to type T
    ///Leave the parameter empty to return all items that can be cast to type T
    ///\tparam T The type of the items. Could be the type of a derived class
    ///\returns Returns a list of all found items.
    template<typename T> inline QList<T*> findAll(QVariant criterion = QVariant(), const char* propertyName = "") const
    { return findItems<T>(this, criterion, propertyName, -1); }

    ///\brief Returns all childs that matches the criterion of the given property and that can be cast to type T
    ///Leave the parameter empty to return all childs that can be cast to type T
    ///\tparam T The type of the items. Could be the type of a derived class
    ///\returns Returns a list of all found items.
    template<typename T> inline QList<T*> findAllChilds(QVariant criterion = QVariant(), const char* propertyName = "") const
    { return findItems<T>(this, criterion, propertyName, 1); }

    ///\brief Returns the root item of the node, if it can be cast to type T
    ///\tparam T The type of the root item. Could be the type of a derived class
    ///\returns Returns a pointer to the root item or 0 if the root item couldn't be cast
    template<typename T> inline T* findRoot() const
    { return parentItem() == 0 ? dynamic_cast<T*>(const_cast<AbstractTreeItem*>(this)) : parentItem()->findRoot<T>(); }

protected:
    QList<AbstractTreeItem*> m_childItems;	///<Child item list
    AbstractTreeItem* m_parentItem;		    ///<Parent item

    virtual bool lessThan(const QVariant& left, const QVariant& right);
    virtual void readXmlTags(QXmlStreamReader& reader);
    virtual void writeXmlTags(QXmlStreamWriter& writer) const;
    virtual void writeItemAttributes(QXmlStreamWriter& writer) const;

signals:
    void parentItemChanged();
    void childsChanged();
};

QML_DECLARE_TYPE(AbstractTreeItem)

///\brief Searches the given item and its childs for the first item that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\param depth The search depth of the tree (negative value means infinite depth)
///\return The first item that matches the criterion or 0 if no item matches
template<typename T> 
inline T* findItem(const AbstractTreeItem* item, QVariant criterion, const char* propertyName, int depth)
{
    //search first item
    if(item->property(propertyName) == criterion) {
        T* foundItem = dynamic_cast<T*>(const_cast<AbstractTreeItem*>(item));
        if(foundItem != 0) return foundItem;
    }

    //break if depth = 0 is reached
    if(depth == 0) return 0;

    //search childs of the item
    for(int i = 0; i<item->childCount(); i++) {
        T* foundItem = findItem<T>(item->child(i), criterion, propertyName, depth-1);
        if(foundItem != 0) return foundItem;
    }

    return 0; //return 0 if the item was not found in any childs
}

///\brief Searches the given item and its childs for items that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\return A list of items that matches the criterion or an empty list if no item matches
template<typename T> 
inline QList<T*> findItems(const AbstractTreeItem* item, QVariant criterion, const char* propertyName, int depth = -1)
{
    //create list
    QList<T*> items;

    //Wildcard that matches all items
    bool matchAll = !criterion.isValid() && QString(propertyName).isEmpty();

    //search first item
    if( (item->property(propertyName) == criterion) || matchAll ) {
        T* foundItem = dynamic_cast<T*>(const_cast<AbstractTreeItem*>(item));
        if(foundItem != 0) items.append( foundItem);
    }

    //break if depth = 0 is reached
    if(depth == 0) return items;

    //search childs of the item
    for(int i = 0; i<item->childCount(); i++) {
        items.append( findItems<T>(item->child(i), criterion, propertyName, depth-1) );
    }

    return items; //return the list
}

#endif
