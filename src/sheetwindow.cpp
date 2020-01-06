#include <QToolBar>

#include "sheetwindow.h"
#include "ui_sheetwindow.h"

SheetWindow::SheetWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SheetWindow)
{
    ui->setupUi(this);
    setupActions();
    createToolBars();
}

SheetWindow::~SheetWindow()
{
    delete ui;
}

void SheetWindow::setupActions()
{
    ui->newSheetAction->setIcon(QIcon::fromTheme("window-new"));
    ui->newSheetAction->setStatusTip(tr("Create a blank sheet in a new window"));
    ui->newSheetAction->setShortcut(QKeySequence::New);
    //connect.....

    ui->openAction->setIcon(QIcon::fromTheme("document-open"));
    ui->openAction->setStatusTip(tr("Open a sheet in current window"));
    ui->openAction->setShortcut(QKeySequence::Open);
    //connect.....

    // TODO: recent files actions

    ui->saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    ui->saveAsAction->setStatusTip(tr("Save the sheet as a new file"));
    ui->saveAsAction->setShortcut(QKeySequence::SaveAs);
    //connect.....

    ui->clearSheetAction->setIcon(QIcon::fromTheme("edit-clear"));
    ui->clearSheetAction->setStatusTip(tr("Remove all meals from the sheet"));
    //connect.....

    ui->refreshAction->setIcon(QIcon::fromTheme("view-refresh"));
    ui->refreshAction->setStatusTip(tr("Reload sheet from disk"));
    ui->refreshAction->setShortcut(QKeySequence::Refresh);
    //connect.....

    ui->closeAction->setIcon(QIcon::fromTheme("window-close"));
    ui->closeAction->setStatusTip(tr("Close this window"));
    ui->closeAction->setShortcut(QKeySequence::Close);
    //connect.....

    ui->quitAction->setIcon(QIcon::fromTheme("application-exit"));
    ui->quitAction->setStatusTip(tr("Close all windows and exit"));
    ui->quitAction->setShortcut(QKeySequence::Quit);
    //connect.....
}

void SheetWindow::createToolBars()
{
    sheetToolBar = addToolBar(tr("&Sheet"));
    sheetToolBar->addAction(ui->newSheetAction);
    sheetToolBar->addAction(ui->openAction);
}
