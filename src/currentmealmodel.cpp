#include <QDebug>
#include <QSqlError>

#include "currentmealmodel.h"


CurrentMealModel::CurrentMealModel(const QString &dbConnectionName, QObject *parent)
    : QSqlQueryModel(parent), m_query(QSqlDatabase::database(dbConnectionName))
{
    m_query.prepare("SELECT mealcontents.id, fooditems.name, mealcontents.weight, "
                    "fooditems.carbs, fooditems.fat, fooditems.protein, fooditems.kcal "
                    "FROM mealcontents, meals, fooditems "
                    "WHERE mealcontents.mealid = meals.id "
                    "AND mealcontents.foodid = fooditems.id "
                    "AND meals.id = :mealId");
    // Do a "dummy" populate to allow view to initialize
    m_query.bindValue(":mealId", 0);
    m_query.exec();
    setQuery(m_query);

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Food Item"));
    setHeaderData(2, Qt::Horizontal, tr("Weight"));
    setHeaderData(3, Qt::Horizontal, tr("Carbs"));
    setHeaderData(4, Qt::Horizontal, tr("Fat"));
    setHeaderData(5, Qt::Horizontal, tr("Protein"));
    setHeaderData(6, Qt::Horizontal, tr("Kcal"));
}

QVariant CurrentMealModel::data(const QModelIndex &idx, int role) const
{
    // If requested index is in carbs, fat, protein or kcal column, we
    // intercept and account for the weight of the food item.
    if (role == Qt::DisplayRole && 3 <= idx.column() && idx.column() <= 6) {
        QModelIndex weightIdx = index(idx.row(), 2);
        double weightFactor = weightIdx.data().toInt() / 100.0;
        double nutritionalValue = QSqlQueryModel::data(idx, role).toDouble();
        return QString::number(nutritionalValue * weightFactor, 'f', 1);
    }
    return QSqlQueryModel::data(idx, role);
}

void CurrentMealModel::populate(int mealId)  // TODO: rename to loadMeal perhaps
{
    m_query.bindValue(":mealId", mealId);
    m_query.exec();
    // Apparently the model doesn't know about the query change here,
    // so we let it know by resetting the query (emitting the layoutChanged
    // signal is an alternative, but it doesn't feel quite right).
    setQuery(m_query);
//    emit QAbstractItemModel::layoutChanged();
}
