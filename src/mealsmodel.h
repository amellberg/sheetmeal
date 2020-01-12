#ifndef MEALSMODEL_H
#define MEALSMODEL_H

#include <QSqlTableModel>

class MealsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    MealsModel(QSqlDatabase db, QObject *parent = nullptr);

public slots:
    void onNewMeal();
    void onMealNameChanged(int row, const QString &mealName);
};

#endif // MEALSMODEL_H
