#ifndef MEALSMODEL_H
#define MEALSMODEL_H

#include <QSqlTableModel>

/**
 * @brief Supplies data for the QComboBox showing meals in the main
 * window. Connected to table 'meals' in the sheet db.
 */
class MealsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    MealsModel(QSqlDatabase db, QObject *parent = nullptr);

public slots:
    void onDeleteMeal(int row);
    void onNewMeal();
    void onMealNameChanged(int row, const QString &newMealName);
};

#endif // MEALSMODEL_H
