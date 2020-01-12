#ifndef MEALTOOLBAR_H
#define MEALTOOLBAR_H

#include <QToolBar>

class QAbstractItemModel;
class QComboBox;

class MealToolBar : public QToolBar
{
    Q_OBJECT
public:
    MealToolBar(QAbstractItemModel *mealsModel, QWidget *parent = nullptr);

public slots:
    void onNewMeal();

private:
    QComboBox *m_mealComboBox;
};

#endif // MEALTOOLBAR_H
