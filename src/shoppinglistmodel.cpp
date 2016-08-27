#include "shoppinglistmodel.h"

#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ShoppingListModel::ShoppingListModel(QObject *parent)
    : QAbstractListModel(parent), m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_db.setDatabaseName("shoppingArticles");
    openFromDevice();
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
            role > MyRoles::CountRole)
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
    case MyRoles::SelectedRole:
        return item.selected;
    case MyRoles::CountRole:
        return item.count;
    }

    return QVariant();
}

bool ShoppingListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row() >= m_shoppingList.count() ||
            (role != MyRoles::BoughtRole &&
             role != MyRoles::ArticleRole &&
             role != MyRoles::InfoTextRole &&
             role != MyRoles::SelectedRole &&
             role != MyRoles::CountRole))
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
    case MyRoles::SelectedRole:
        item.selected = value.toBool();
        emit dataChanged(index, index, {MyRoles::SelectedRole});
        return true;
    case MyRoles::CountRole:
        item.count = value.toInt();
        emit dataChanged(index, index, {MyRoles::CountRole});
        return true;
    }

    return false;
}

Qt::ItemFlags ShoppingListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void ShoppingListModel::openFromDevice()
{
    if(m_db.open())
    {
        //Create table if not exist
        QSqlQuery createTableQuery(m_db);
        if(!createTableQuery.exec("CREATE TABLE IF NOT EXISTS ShoppingArticles "
                                  "(Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "Article TEXT, "
                                  "Count INTEGER, "
                                  "Info TEXT, "
                                  "Bought INTEGER);"))
        {
            emit errorHappend(tr("Datenbankfehler"),
                              tr("Fehler beim Erzeugen der Datenbank! %1")
                              .arg(createTableQuery.lastError().text()));
            m_db.close();
            return;
        }



        QSqlQuery readQuery(m_db);
        QString read = QString("SELECT * FROM ShoppingArticles");

        if(!readQuery.prepare(read))
        {
            emit errorHappend(tr("Datenbankfehler"),
                              tr("Fehler beim Vorbereiten der Query! %1")
                              .arg(readQuery.lastError().text()));
            m_db.close();
            return;
        }

        if(readQuery.exec())
        {
            while (readQuery.next())
            {
                ShoppingItem item;
                item.id = readQuery.value("Id").toInt();
                item.article = readQuery.value("Article").toString();
                item.count = readQuery.value("Count").toInt();
                item.bought = readQuery.value("Bought").toInt() > 0;
                item.info = readQuery.value("Info").toString();
                m_shoppingList.append(item);
            }
        }
        else
        {
            emit errorHappend(tr("Datenbankfehler"),
                              tr("Fehler beim Ausführen der Query! %1")
                              .arg(readQuery.lastError().text()));
            m_db.close();
            return;
        }

        m_db.close();
    }
    else
    {
        emit errorHappend(tr("Datenbankfehler"),
                          tr("Fehler beim Öffnen der Datenbank!"));
    }
}

void ShoppingListModel::saveToDevice()
{
    if(m_db.open())
    {
        //Create table if not exist
        QSqlQuery createTableQuery(m_db);
        if(!createTableQuery.exec("CREATE TABLE IF NOT EXISTS ShoppingArticles "
                                  "(Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "Article TEXT, "
                                  "Count INTEGER, "
                                  "Info TEXT, "
                                  "Bought INTEGER);"))
        {
            emit errorHappend(tr("Datenbankfehler"),
                              tr("Fehler beim Erzeugen der Datenbank! %1")
                              .arg(createTableQuery.lastError().text()));
            m_db.close();
            return;
        }

        //clear current table
        QSqlQuery deleteQuery(m_db);
        if(!deleteQuery.exec("DELETE FROM ShoppingArticles;"))
        {
            emit errorHappend(tr("Datenbankfehler"),
                              tr("Fehler beim Löschen der Datenbank! %1")
                              .arg(deleteQuery.lastError().text()));
            m_db.close();
            return;
        }

        for(const ShoppingItem &item: m_shoppingList)
        {
            QSqlQuery query(m_db);
            QString update = QString("INSERT OR REPLACE INTO ShoppingArticles "
                           "(Id, Article, Count, Info, Bought) "
                           "VALUES(:Id, :Article, :Count, :Info, :Bought)");

            if(!query.prepare(update))
            {
                emit errorHappend(tr("Datenbankfehler"),
                                  tr("Fehler beim Vorbereiten der Query! %1")
                                  .arg(query.lastError().text()));
                m_db.close();
                return;
            }

            query.bindValue(":Id", item.id);
            query.bindValue(":Article", item.article);
            query.bindValue(":Count", item.count);
            query.bindValue(":Info", item.info);
            query.bindValue(":Bought", item.bought ? 1 : 0);

            if(!query.exec())
            {
                emit errorHappend(tr("Datenbankfehler"),
                                  tr("Fehler beim Ausführen der Query! %1")
                                  .arg(query.lastError().text()));
                m_db.close();
                return;
            }
        }

        emit infoHappend(tr("Datenbank"),
                         tr("Datenbank wurde aktualisiert!"));

        m_db.close();
    }
    else
    {
        emit errorHappend(tr("Datenbankfehler"),
                          tr("Fehler beim Öffnen der Datenbank!"));
    }
}

void ShoppingListModel::insertArticle(const QString &article,
                                      const QString &info, qint64 count)
{
    emit beginInsertRows(QModelIndex(), m_shoppingList.count(),
                         m_shoppingList.count());
    ShoppingItem item;
    item.article = article;
    item.info = info;
    item.count = count;
    item.bought = false;
    item.id = m_shoppingList.count();
    item.selected = false;

    m_shoppingList.append(item);
    emit endInsertRows();
}

void ShoppingListModel::removeSelectedItems()
{
    bool removed = false;
    for(int index = 0; index < m_shoppingList.count(); index++)
    {
        if(m_shoppingList.at(index).selected)
        {
            emit beginRemoveRows(QModelIndex(), index, index);
            m_shoppingList.removeAt(index);
            removed = true;
            emit endRemoveRows();
        }
    }

    if(!removed)
    {
        emit errorHappend(tr("Fehler"),
                          tr("Keine Produkte ausgewählt!"));
    }
}
void ShoppingListModel::removeAllItems()
{
    emit beginResetModel();
    m_shoppingList.clear();
    emit endResetModel();
}

QHash<int, QByteArray> ShoppingListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MyRoles::IdRole] = "id";
    roles[MyRoles::ArticleRole] = "article";
    roles[MyRoles::BoughtRole] = "bought";
    roles[MyRoles::InfoTextRole] = "infotext";
    roles[MyRoles::SelectedRole] = "selected";
    roles[MyRoles::CountRole] = "count";
    return roles;
}
