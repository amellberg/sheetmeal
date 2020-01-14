#ifndef SHEETWINDOW_H
#define SHEETWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SheetWindow;
}

class QComboBox;
class MealsModel;
class MealToolBar;

class SheetWindow : public QMainWindow
{
    Q_OBJECT

public:
    SheetWindow(QString sheetPath = QString(""), QWidget *parent = nullptr);
    ~SheetWindow();

private slots:
    void onNewMeal();
    void onRenameMeal();
    void onMealModelReset();

signals:
    void mealNameChanged(int row, const QString &newMealName);

private:
    bool createTemporarySheetPath();
    bool createDatabaseConnection();
    void initializeSheet();

    void setupActions();
    void createToolBars();
    void createModels();

    Ui::SheetWindow *m_ui;
    QToolBar *m_sheetToolBar;
    QToolBar *m_mealsToolBar;
    QComboBox *m_mealsComboBox;

    MealsModel *m_mealsModel;
    int lastMealIndex;
    bool restoreMealIndex;

    QString m_connectionName;  // DB conn. name, unique to each running sheet
    QString m_sheetPath;
};

#endif // SHEETWINDOW_H
