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
    SheetWindow(QWidget *parent = nullptr);
    ~SheetWindow();

private:
    void setupActions();
    void createToolBars();

    Ui::SheetWindow *ui;
    QToolBar *sheetToolBar;
    QComboBox *mealComboBox;
};

#endif // SHEETWINDOW_H
