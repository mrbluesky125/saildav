#ifndef ABSTRACT_TREE_ITEM_H
#define ABSTRACT_TREE_ITEM_H

#include <QtDeclarative>

#include <QtCore>
#include <QtDebug>

class AbstractTreeItem;

template<typename T> inline T* findItem(const AbstractTreeItem* item, QVariant criterion, const char* propertyName);
template<typename T> inline QList<T*> findItems(const AbstractTreeItem* item, QVariant criterion, const char* propertyName);

///\brief Provides a tree item that handles the basic tree structure.
///With a QML view you can work directly on the properties of the item and don't have
///to worry about any data/setData functions.
///You can also use the 'childs' property to display the childs in a QML list view.
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
    AbstractTreeItem* takeChild(int position);
    
    virtual Qt::ItemFlags flags() const { return Qt::ItemIsEnabled | Qt::ItemIsSelectable; }
    virtual int columnCount() const { return 1; }

    template<typename T>
    inline T* findFirst(QVariant criterion, const char* propertyName) const
    { return findItem<T>(this, criterion, propertyName); }

    template<typename T>
    inline QList<T*> findAll(QVariant criterion, const char* propertyName) const
    { return findItems<T>(this, criterion, propertyName); }
    
    template<typename T>
    inline QList<T*> findAll() const
    { return findItems<T>(this, QVariant(), ""); }

    template<typename T>
    inline T* findRoot() const
    { return parentItem() == 0 ? dynamic_cast<T*>(const_cast<AbstractTreeItem*>(this)) : parentItem()->findRoot<T>(); }

protected:
    QList<AbstractTreeItem*> m_childItems;	///<Child item list
    AbstractTreeItem* m_parentItem;		///<Parent item

signals:
    void parentItemChanged(AbstractTreeItem*);
    void childsChanged();
    void indentChanged(unsigned int);

};

QML_DECLARE_TYPE(AbstractTreeItem)

///\brief Searches the given item and its childs for the first item that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\return The first item that matches the criterion or 0 if no item matches
template<typename T> 
inline T* findItem(const AbstractTreeItem* item, QVariant criterion, const char* propertyName)
{
    //search first item
    if(item->property(propertyName) == criterion)
        return dynamic_cast<T*>(const_cast<AbstractTreeItem*>(item));

    //search childs of the item
    for(unsigned int i = 0; i<item->childCount(); i++) {
        T* childItem = findItem<T>(item->child(i), criterion, propertyName);
        if(childItem != 0)
            return childItem;	//return the item if found in the child
    }

    return 0; //return 0 if the item was not found in any childs
}

///\brief Searches the given item and its childs for items that matches the given criterion with the given property name.
///\param criterion criterion that is searched for
///\param propertyName The name of the property that is probed for the criterion
///\return A list of items that matches the criterion or an empty list if no item matches
template<typename T> 
inline QList<T*> findItems(const AbstractTreeItem* item, QVariant criterion, const char* propertyName)
{
    //create list
    QList<T*> items;

    //search first item
    if(item->property(propertyName) == criterion || (!criterion.isValid()) )
        items.append( dynamic_cast<T*>(const_cast<AbstractTreeItem*>(item)) );

    //search childs of the item
    for(unsigned int i = 0; i<item->childCount(); i++) {
        items.append( findItems<T>(item->child(i), criterion, propertyName) );
    }

    return items; //return the list
}

#endif
