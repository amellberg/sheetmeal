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

void MealsModel::onMealNameChanged(int row, const QString &mealName)
{
    QModelIndex idx = index(row, 1);
    QSqlRecord rec = record(idx.row());
//    qDebug() << rec;
    rec.setValue("name", mealName);
    setRecord(idx.row(), rec);
//    qDebug() << ret;
    submitAll();
//    select();
}
