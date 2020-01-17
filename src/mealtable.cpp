#include <QHeaderView>

#include "mealtable.h"


MealTable::MealTable(QAbstractItemModel *model, QWidget *parent)
    : QTableView(parent)
{
    setModel(model);
    verticalHeader()->hide(); // TODO: unhide and put checkboxes in vertical header
    setGridStyle(Qt::DashLine);
    setAlternatingRowColors(true);
    setSelectionBehavior(SelectionBehavior::SelectRows);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(24);

    // Hide ID column and let the Food Item column stretch
    setColumnHidden(0, true);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

//    for (int i = 2; i < mealsModel->columnCount(); i++) {
//        setColumnWidth(i, 75);
//    }
}
