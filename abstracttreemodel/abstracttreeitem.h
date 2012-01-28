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

#ifndef ABSTRACT_TREE_ITEM_H
#define ABSTRACT_TREE_ITEM_H

#include <QtDeclarative>

#include <QtCore>
#include <QtDebug>

///\brief Provides a tree item that handles the basic tree structure.
///The data() and setData() (for editable models) functions should be
///reimplemented in derived classes if you use a standard widget view. However with
///QML views you can work directly on the properties of the item and don't have
///to worry about the data functions.
///
///\remarks This class is basically intended for just 1 column, more columns
///used in QTreeView requires further considerations :-) (e.g. you can set/get
///particular properties for other columns in the data access functions)
class AbstractTreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AbstractTreeItem* parentItem READ parentItem WRITE setParentItem)
    Q_PROPERTY(QDeclarativeListProperty<AbstractTreeItem> childs READ childs NOTIFY childsChanged)
    Q_PROPERTY(unsigned int indent READ indent NOTIFY indentChanged)

public:
    AbstractTreeItem(AbstractTreeItem *parentItem = 0);
    virtual ~AbstractTreeItem();

    AbstractTreeItem* child(int number) const;
    QList<AbstractTreeItem*> childList();

    QDeclarativeListProperty<AbstractTreeItem> childs();
    static void append(QDeclarativeListProperty<AbstractTreeItem>* list, AbstractTreeItem* item);
    static int count(QDeclarativeListProperty<AbstractTreeItem>* list);
    static AbstractTreeItem* at(QDeclarativeListProperty<AbstractTreeItem>* list, int index);
    
    AbstractTreeItem* parentItem() const;
    void setParentItem(AbstractTreeItem* parentItem);

    unsigned int indent() const;

    unsigned int childCount() const;
    unsigned int childNumber() const;
    unsigned int itemCount() const;
    bool addChild(AbstractTreeItem* item);
    bool insertChild(int position, AbstractTreeItem* item);
    bool insertChildren(int position, const QList<AbstractTreeItem*>& items);
    bool moveChild(int from, int to);
    bool removeChild(AbstractTreeItem* item);
    bool removeChildren(int position, int count);
    
    virtual Qt::ItemFlags flags() const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable; }
    virtual int columnCount() const { return 1; }
    virtual QVariant data(int column, int role = Qt::DisplayRole) const { return QString("AbtractTreeItem"); }
    virtual bool setData(int column, const QVariant &value, int role = Qt::DisplayRole) { return false; }

    AbstractTreeItem* findFirst(QVariant criterion, const char* propertyName) const;
    QList<AbstractTreeItem*> findAll(QVariant criterion, const char* propertyName) const;
    QList<AbstractTreeItem*> findAll() const;

protected:
    QList<AbstractTreeItem*> m_childItems;	///<Child item list
    AbstractTreeItem* m_parentItem;		///<Parent item

signals:
    void parentItemChanged(AbstractTreeItem*);
    void childsChanged();
    void indentChanged(unsigned int);

};

#endif
