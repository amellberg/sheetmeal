#ifndef MEALTABLE_H
#define MEALTABLE_H

#include <QTableView>

class QAbstractItemModel;

class MealTable : public QTableView
{
    Q_OBJECT
public:
    MealTable(QAbstractItemModel *model, QWidget *parent = nullptr);
};

#endif // MEALTABLE_H
