#include <QSqlRecord>
#include <QModelIndex>
#include <QDebug>

#include "mealsmodel.h"


MealsModel::MealsModel(QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db)
{
    setTable("meals");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void MealsModel::onNewMeal()
{
    QSqlRecord rec = record();
    rec.setValue("name", "Untitled");
    insertRecord(-1, rec);
    submitAll();
}

void MealsModel::onMealNameChanged(int row, const QString &newMealName)
{
    QModelIndex idx = index(row, fieldIndex("name"));
    QSqlRecord rec = record(idx.row());
    rec.setValue("name", newMealName);
    setRecord(idx.row(), rec);
    submitAll();
}
