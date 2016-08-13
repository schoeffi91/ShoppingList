#include "shoppinglistmodel.h"

ShoppingListModel::ShoppingListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    //TODO remove this

    ShoppingItem item;
    item.id = 0;
    item.article = "Mein ARtikel 1";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 1;
    item.article = "Mein ARtikel 2";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 2;
    item.article = "Mein ARtikel 3";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 3;
    item.article = "Mein ARtikel 4";
    item.bought = false;
    item.info = "DAs ist meine info .... info. ... info ...infof. ...finf ";
    m_shoppingList.append(item);

    item.id = 4;
    item.article = "Mein ARtikel 5";
    item.bought = true;
    m_shoppingList.append(item);

    item.id = 0;
    item.article = "Mein ARtikel 1";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 1;
    item.article = "Mein ARtikel 2";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 2;
    item.article = "Mein ARtikel 3";
    item.bought = false;
    m_shoppingList.append(item);

    item.id = 3;
    item.article = "Mein ARtikel 4";
    item.bought = false;
    item.info = "DAs ist meine info .... info. ... info ...infof. ...finf ";
    m_shoppingList.append(item);

    item.id = 4;
    item.article = "Mein ARtikel 5";
    item.bought = true;
    m_shoppingList.append(item);
}

int ShoppingListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_shoppingList.count();
}

QVariant ShoppingListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= m_shoppingList.count() ||
            role < MyRoles::IdRole ||
            role > MyRoles::BoughtRole)
    {
        return QVariant();
    }

    const ShoppingItem &item = m_shoppingList.at(index.row());

    switch(role)
    {
    case MyRoles::IdRole:
        return item.id;
    case MyRoles::BoughtRole:
        return item.bought;
    case MyRoles::ArticleRole:
        return item.article;
    case MyRoles::InfoTextRole:
        return item.info;
    }

    return QVariant();
}

bool ShoppingListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row() >= m_shoppingList.count() ||
            (role != MyRoles::BoughtRole &&
             role != MyRoles::ArticleRole &&
             role != MyRoles::InfoTextRole))
    {
        return false;
    }

    ShoppingItem &item = m_shoppingList[index.row()];

    switch(role)
    {
    case MyRoles::BoughtRole:
        item.bought = value.toBool();
        emit dataChanged(index, index, {MyRoles::BoughtRole});
        return true;
    case MyRoles::ArticleRole:
        item.article = value.toString();
        emit dataChanged(index, index, {MyRoles::ArticleRole});
        return true;
    case MyRoles::InfoTextRole:
        item.info = value.toString();
        emit dataChanged(index, index, {MyRoles::InfoTextRole});
        return true;
    }

    return false;
}

Qt::ItemFlags ShoppingListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ShoppingListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MyRoles::IdRole] = "id";
    roles[MyRoles::ArticleRole] = "article";
    roles[MyRoles::BoughtRole] = "bought";
    roles[MyRoles::InfoTextRole] = "infotext";
    return roles;
}
