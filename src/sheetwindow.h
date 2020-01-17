#ifndef SHEETWINDOW_H
#define SHEETWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SheetWindow;
}

class QComboBox;
class MealTable;
class TotalsTable;
class MealsModel;
class CurrentMealModel;
class MealToolBar;


class SheetWindow : public QMainWindow
{
    Q_OBJECT

public:
    SheetWindow(QString sheetPath = QString(""), QWidget *parent = nullptr);
    ~SheetWindow();

private slots:
    void onDeleteMeal();
    void onNewMeal();
    void onRenameMeal();
    void onMealsModelReset();

signals:
    void mealDeleted(int row);
    void mealNameChanged(int row, const QString &newMealName);

private:
    bool createTemporarySheetPath();
    bool createDatabaseConnection();
    void initializeSheet();

    void setupActions();
    void createToolBars();
    void createMainLayout();
    void createModels();

    void sheetIsEmpty();
    void sheetIsNonEmpty();

    Ui::SheetWindow *m_ui;
    MealTable *m_mealTable;
    TotalsTable *m_totalsTable;
    QToolBar *m_sheetToolBar;
    QToolBar *m_mealsToolBar;
    QComboBox *m_mealsComboBox;

    MealsModel *m_mealsModel;
    int m_storedMealIndex;    // For restoring/adjusting selected meal after model reset
    bool m_restoreMealIndex;  // "
    CurrentMealModel *m_currentMealModel;

    QString m_dbConnectionName;  // Unique to each running sheet
    QString m_sheetPath;         // Path to sheet DB file
};

#endif // SHEETWINDOW_H
