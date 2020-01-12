#include <QComboBox>
#include <QDebug>

#include "mealtoolbar.h"

MealToolBar::MealToolBar(QAbstractItemModel *mealsModel, QWidget *parent)
    : QToolBar(parent)
{
    m_mealComboBox = new QComboBox;
    m_mealComboBox->setModel(mealsModel);
    m_mealComboBox->setModelColumn(1);  // Display 'name' column
    m_mealComboBox->setMinimumWidth(125);
//    mealComboBox->setEnabled(false);
    addWidget(m_mealComboBox);
}

void MealToolBar::onNewMeal()
{
    m_mealComboBox->setCurrentIndex(m_mealComboBox->model()->rowCount() - 1);
}
