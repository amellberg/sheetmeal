#include <QComboBox>

#include "mealtoolbar.h"

MealToolBar::MealToolBar(QWidget *parent)
    : QToolBar(parent)
{
    m_mealComboBox = new QComboBox;
    m_mealComboBox->setMinimumWidth(125);
//    mealComboBox->setEnabled(false);
    addWidget(m_mealComboBox);
}
