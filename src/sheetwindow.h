#ifndef SHEETWINDOW_H
#define SHEETWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SheetWindow;
}

class SheetWindow : public QMainWindow
{
    Q_OBJECT

public:
    SheetWindow(QWidget *parent = nullptr);
    ~SheetWindow();

private:
    void setupActions();
    void createToolBars();

    QToolBar *sheetToolBar;
    Ui::SheetWindow *ui;
};

#endif // SHEETWINDOW_H
