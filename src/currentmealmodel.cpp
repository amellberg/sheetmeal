#include "currentmealmodel.h"


CurrentMealModel::CurrentMealModel(const QString &dbConnectionName, QObject *parent)
    : QSqlQueryModel(parent), m_dbConnectionName(dbConnectionName)
{
    QString sql = "SELECT mealcontents.id, fooditems.name, mealcontents.weight, "
                  "fooditems.carbs, fooditems.fat, fooditems.protein, fooditems.kcal "
                  "FROM mealcontents, meals, fooditems "
                  "WHERE mealcontents.mealid = meals.id "
                  "AND mealcontents.foodid = fooditems.id "
                  "AND meals.id = 2";

    setQuery(sql, QSqlDatabase::database(m_dbConnectionName));
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
    if (role == Qt::DisplayRole && 3 <= idx.column() && idx.column() <= 6) {
        QModelIndex weightIdx = index(idx.row(), 2);
        double weightFactor = weightIdx.data().toInt() / 100.0;
        double nutritionalValue = QSqlQueryModel::data(idx, role).toDouble();
        return QString::number(nutritionalValue * weightFactor, 'f', 1);
    }
    return QSqlQueryModel::data(idx, role);
}
