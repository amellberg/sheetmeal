#include "currentmealmodel.h"


CurrentMealModel::CurrentMealModel(const QString &dbConnectionName, QObject *parent)
    : QSqlQueryModel(parent), m_dbConnectionName(dbConnectionName)
{

}
