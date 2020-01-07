#include <QComboBox>
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
    // --- Sheet actions ---

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
    ui->closeAction->setStatusTip(tr("Close this sheet"));
    ui->closeAction->setShortcut(QKeySequence::Close);
    //connect.....

    ui->quitAction->setIcon(QIcon::fromTheme("application-exit"));
    ui->quitAction->setStatusTip(tr("Close all sheets and exit"));
    ui->quitAction->setShortcut(QKeySequence::Quit);
    //connect.....


    // --- Meal actions ---

    ui->newMealAction->setIcon(QIcon::fromTheme("document-new"));
    ui->newMealAction->setStatusTip(tr("Add a new meal to the sheet"));
    ui->newMealAction->setShortcut(QKeySequence::fromString("ctrl+shift+n"));

    ui->duplicateAction->setIcon(QIcon::fromTheme("edit-copy"));
    ui->duplicateAction->setStatusTip(tr("Duplicate the current meal"));

    ui->deleteAction->setIcon(QIcon::fromTheme("edit-delete"));
    ui->deleteAction->setStatusTip(tr("Remove the meal from the sheet"));

    ui->clearMealAction->setIcon(QIcon::fromTheme("edit-clear"));
    ui->clearMealAction->setStatusTip(tr("Remove all food items from the meal"));

    ui->addFoodAction->setIcon(QIcon::fromTheme("list-add"));
    ui->addFoodAction->setStatusTip(tr("Add food to the meal"));
    ui->addFoodAction->setShortcut(QKeySequence::fromString("insert"));
}

void SheetWindow::createToolBars()
{
    sheetToolBar = addToolBar(tr("&Sheet"));
    sheetToolBar->addAction(ui->newSheetAction);
    sheetToolBar->addAction(ui->openAction);
    sheetToolBar->addAction(ui->saveAsAction);
//    sheetToolBar->addSeparator();
//    sheetToolBar->addAction(ui->clearSheetAction);
    sheetToolBar->addAction(ui->refreshAction);
}
