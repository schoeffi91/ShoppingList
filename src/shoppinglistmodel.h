#ifndef SHOPPINGLISTMODEL_H
#define SHOPPINGLISTMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QSqlDatabase>

class ShoppingListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ShoppingListModel(QObject *parent = 0);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex())const override final;

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override final;

    Q_INVOKABLE void openFromDevice();

    Q_INVOKABLE void saveToDevice();

    Q_INVOKABLE void insertArticle(const QString &article, const QString &info, qint64 count);

    Qt::ItemFlags flags(const QModelIndex &index) const override final;

    Q_INVOKABLE void removeSelectedItems();

    Q_INVOKABLE void removeAllItems();

    QHash<int, QByteArray> roleNames() const override final;

    enum MyRoles
    {
        IdRole = Qt::UserRole + 1,
        ArticleRole,
        InfoTextRole,
        BoughtRole,
        SelectedRole
    };

signals:
    void errorHappend(const QString &title, const QString &message);
    void infoHappend(const QString &title, const QString &message);

private:
    struct ShoppingItem
    {
        qint64 id = 0;
        bool bought = false;
        QString info;
        QString article;
        qint64 count = 0;
        bool selected = false;
    };

    QList<ShoppingItem> m_shoppingList;

    QSqlDatabase m_db;
};

#endif // SHOPPINGLISTMODEL_H
