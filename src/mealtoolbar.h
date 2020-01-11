#ifndef MEALTOOLBAR_H
#define MEALTOOLBAR_H

#include <QToolBar>

class QComboBox;

class MealToolBar : public QToolBar
{
    Q_OBJECT
public:
    MealToolBar(QWidget *parent = nullptr);

private:
    QComboBox *m_mealComboBox;
};

#endif // MEALTOOLBAR_H
