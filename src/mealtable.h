#ifndef MEALTABLE_H
#define MEALTABLE_H

#include <QTableView>

class MealTable : public QTableView
{
    Q_OBJECT
public:
    MealTable(QWidget *parent = nullptr);
};

#endif // MEALTABLE_H
