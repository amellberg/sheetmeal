#include "sheetwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SheetWindow *sheet = new SheetWindow;
    sheet->show();
    return app.exec();
}
