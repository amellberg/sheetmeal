#include <QApplication>

#include "sheetwindow.h"


int main(int argc, char *argv[])
{
//    QApplication::setStyle("fusion");
    QApplication app(argc, argv);
    SheetWindow *sheet = new SheetWindow("/home/akem/Code/sheetmeal/dummy.sheet");
    sheet->show();
    return app.exec();
}
