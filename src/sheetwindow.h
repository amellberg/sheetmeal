#ifndef SHEETWINDOW_H
#define SHEETWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SheetWindow;
}

class QComboBox;

class SheetWindow : public QMainWindow
{
    Q_OBJECT

public:
    SheetWindow(QString sheetPath = QString(""), QWidget *parent = nullptr);
    ~SheetWindow();

private:
    bool createSheetConnection(QString sheetPath);
    void setupActions();
    void createToolBars();

    Ui::SheetWindow *ui;
    QToolBar *sheetToolBar;
    QToolBar *mealToolBar;
    QComboBox *mealComboBox;
};

#endif // SHEETWINDOW_H
