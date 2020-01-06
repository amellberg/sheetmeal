#include "sheetwindow.h"
#include "ui_sheetwindow.h"

SheetWindow::SheetWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SheetWindow)
{
    ui->setupUi(this);
}

SheetWindow::~SheetWindow()
{
    delete ui;
}
