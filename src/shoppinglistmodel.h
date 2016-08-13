#ifndef SHOPPINGLISTMODEL_H
#define SHOPPINGLISTMODEL_H

#include <QAbstractListModel>
#include <QHash>

class ShoppingListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ShoppingListModel(QObject *parent = 0);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex())const override final;

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override final;

    Qt::ItemFlags flags(const QModelIndex &index) const override final;

    QHash<int, QByteArray> roleNames() const override final;

    enum MyRoles
    {
        IdRole = Qt::UserRole + 1,
        ArticleRole,
        InfoTextRole,
        BoughtRole
    };

private:
    struct ShoppingItem
    {
        qint64 id = 0;
        bool bought = false;
        QString info;
        QString article;
    };

    QList<ShoppingItem> m_shoppingList;
};

#endif // SHOPPINGLISTMODEL_H
