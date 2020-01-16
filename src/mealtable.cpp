#include <QHeaderView>

#include "mealtable.h"
//#include "currentmealmodel.h"


MealTable::MealTable(QWidget *parent)
    : QTableView(parent)
{
    verticalHeader()->hide(); // TODO: unhide and put checkboxes in vertical header
    setGridStyle(Qt::DashLine);
    setAlternatingRowColors(true);
    setSelectionBehavior(SelectionBehavior::SelectRows);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(24);

    //    horizontalHeader()->setSectionHidden(0, true);
//    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
//    setColumnHidden(0, true);
    // Hide ID column and let the Food Item column stretch
//    horizontalHeader()->hideSection(0);
//    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

//    for (int i = 2; i < mealsModel->columnCount(); i++) {
//        setColumnWidth(i, 75);
//    }
}
