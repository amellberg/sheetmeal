#ifndef SHEETWINDOW_H
#define SHEETWINDOW_H

#include <QMainWindow>

class SheetWindow : public QMainWindow
{
    Q_OBJECT

public:
    SheetWindow(QWidget *parent = nullptr);
    ~SheetWindow();
};
#endif // SHEETWINDOW_H
