#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTemporaryFile>
#include <QToolBar>
#include <QUuid>

#include "sheetwindow.h"
#include "ui_sheetwindow.h"

SheetWindow::SheetWindow(QString sheetPath, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SheetWindow)
{
    // if default sheet is set, update sheetPath

    if (!createSheetConnection(sheetPath))
        return;

    ui->setupUi(this);
    setupActions();
    createToolBars();
}

SheetWindow::~SheetWindow()
{
    delete ui;
}

bool SheetWindow::createSheetConnection(QString sheetPath)
{
    if (sheetPath.isEmpty()) {
        // Generate a temporary filename for the sheet in system tmp dir
        QTemporaryFile tmpFile(QDir::tempPath() + "/sheet");
        if (!tmpFile.open()) {
            QMessageBox::critical(
                        nullptr, tr("Cannot initialize sheet"),
                        tr("Unable to create a temporary sheet.\n"
                           "Error writing to system's temporary directory."),
                        QMessageBox::Close);
            return false;
        }
        sheetPath = tmpFile.fileName();
    }
    QString connName = QUuid::createUuid().toString();
//    qDebug() << QString("sheetPath = %1").arg(sheetPath);
//    qDebug() << QString(" connName = %1").arg(connName);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    db.setDatabaseName(sheetPath);
    if (!db.open()) {
        QMessageBox::critical(
                    nullptr, tr("Cannot initialize sheet"),
                    tr("Unable to establish a database connection"),
                    QMessageBox::Close);
        return false;
    }
    return true;
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

    ui->setFoodBankAction->setStatusTip(tr("Specify which food bank file to use"));

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
    //connect(ui->newMealAction, &QAction::triggered, mealToolBar, &MealToolBar::)

    ui->duplicateAction->setIcon(QIcon::fromTheme("edit-copy"));
    ui->duplicateAction->setStatusTip(tr("Duplicate the current meal"));

    ui->deleteAction->setIcon(QIcon::fromTheme("edit-delete"));
    ui->deleteAction->setStatusTip(tr("Remove selected meal from the sheet"));
//    ui->deleteAction->setEnabled(false);

    ui->clearMealAction->setIcon(QIcon::fromTheme("edit-clear"));
    ui->clearMealAction->setStatusTip(tr("Remove all food items from the meal"));

    ui->addFoodAction->setIcon(QIcon::fromTheme("list-add"));
    ui->addFoodAction->setStatusTip(tr("Add food items to the selected meal"));
    ui->addFoodAction->setShortcut(QKeySequence::fromString("insert"));
//    ui->addFoodAction->setEnabled(false);

    ui->removeFoodAction->setIcon(QIcon::fromTheme("list-remove"));
    ui->removeFoodAction->setStatusTip(tr("Remove selected food items from meal"));
    ui->removeFoodAction->setShortcut(QKeySequence::fromString("delete"));
//    ui->removeFoodAction->setEnabled(false);
}

void SheetWindow::createToolBars()
{
    sheetToolBar = addToolBar(tr("&Sheet"));
    sheetToolBar->addAction(ui->newSheetAction);
    sheetToolBar->addAction(ui->openAction);
    sheetToolBar->addAction(ui->refreshAction);
//    sheetToolBar->setHidden(true);

    mealToolBar = addToolBar(tr("&Meal"));
    mealComboBox = new QComboBox;
    mealComboBox->setMinimumWidth(125);
//    mealComboBox->setEnabled(false);
    mealComboBox->addItem("Breakfast");
    mealComboBox->addItem("Lunch");
    mealToolBar->addWidget(mealComboBox);
    mealToolBar->addAction(ui->newMealAction);
    mealToolBar->addAction(ui->deleteAction);
    mealToolBar->addAction(ui->addFoodAction);
    mealToolBar->addAction(ui->removeFoodAction);
}

