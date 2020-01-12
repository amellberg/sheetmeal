#include <QSqlRecord>

#include "mealsmodel.h"


MealsModel::MealsModel(QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db)
{
    setTable("meals");
}

void MealsModel::onNewMeal()
{
    QSqlRecord rec = record();
    rec.setValue("name", "Breakfast");
    insertRecord(-1, rec);
    select();
}
