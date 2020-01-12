#include <QComboBox>
#include <QDebug>
#include <QLineEdit>

#include "mealtoolbar.h"

MealToolBar::MealToolBar(QAbstractItemModel *mealsModel, QWidget *parent)
    : QToolBar(parent)
{
    m_mealComboBox = new QComboBox;
    m_mealComboBox->setModel(mealsModel);
    m_mealComboBox->setModelColumn(1);  // Display 'name' column
    m_mealComboBox->setMinimumWidth(125);
    m_mealComboBox->setEditable(true);
    addWidget(m_mealComboBox);

    connect(m_mealComboBox->lineEdit(), &QLineEdit::editingFinished,
            this, &MealToolBar::onMealNameChanged);
}

void MealToolBar::onNewMeal()
{
    m_mealComboBox->setCurrentIndex(m_mealComboBox->model()->rowCount() - 1);
}

void MealToolBar::onMealNameChanged()
{
    emit mealNameChanged(m_mealComboBox->currentIndex(),
                         m_mealComboBox->currentText());
}
