#include "sheetwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    QApplication::setStyle("fusion");
    QApplication app(argc, argv);
    SheetWindow *sheet = new SheetWindow;
    sheet->show();
    return app.exec();
}