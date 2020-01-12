#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryFile>
#include <QToolBar>
#include <QUuid>

#include "mealsmodel.h"
#include "mealtoolbar.h"
#include "sheetwindow.h"
#include "ui_sheetwindow.h"


SheetWindow::SheetWindow(QString sheetPath, QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::SheetWindow), m_sheetPath(sheetPath)
{
    // using QSettings: if default sheet is set, update m_sheetPath here

    if (sheetPath.isEmpty()) {
        if (!createTemporarySheetPath()) {
            delete m_ui;
            return;
        }
    }

    if (!createDatabaseConnection()) {
        delete m_ui;
        return;
    }

    if (sheetPath.isEmpty()) {
        initializeSheet();
    }

    m_ui->setupUi(this);
    createModels();
    createToolBars();
    setupActions();

    connect(m_mealToolBar, &MealToolBar::mealNameChanged,
            m_mealsModel, &MealsModel::onMealNameChanged);
}

SheetWindow::~SheetWindow()
{
    {
        auto db = QSqlDatabase::database(m_connectionName);
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(m_connectionName);

    delete m_ui;
}

bool SheetWindow::createTemporarySheetPath()
{
    // Generate a path for the sheet in system tmp dir
    QTemporaryFile tmpFile(QDir::tempPath() + "/sheet");
    if (!tmpFile.open()) {
        QMessageBox::critical(
                    nullptr, tr("Cannot initialize sheet"),
                    tr("Unable to create a new sheet.\n"
                       "Error writing to system's temporary directory."),
                    QMessageBox::Close);
        return false;
    }
    m_sheetPath = tmpFile.fileName();
    return true;
}

bool SheetWindow::createDatabaseConnection()
{
    m_connectionName = QUuid::createUuid().toString();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    db.setDatabaseName(m_sheetPath);
    if (!db.open()) {
        QMessageBox::critical(
                    nullptr, tr("Cannot initialize sheet"),
                    tr("Unable to establish a database connection."),
                    QMessageBox::Close);
        return false;
    }
    return true;
}

void SheetWindow::initializeSheet()
{
    QSqlQuery query(QSqlDatabase::database(m_connectionName));

    // Table 'sheet' stores some extra data; can contain at most one row
    query.exec("CREATE TABLE sheet ("
               "id INTEGER PRIMARY KEY CHECK (id = 1), "
               "name TEXT NOT NULL, "
               "created TEXT NOT NULL, " // Date format: SQLite's SELECT date("now", "localtime")
               "modified TEXT NOT NULL)");

    query.exec("CREATE TABLE fooditems ("
               "id INTEGER PRIMARY KEY, "
               "name TEXT NOT NULL, "
               "carbs REAL NOT NULL, "
               "fat REAL NOT NULL, "
               "protein REAL NOT NULL, "
               "kcal REAL NOT NULL)");

    query.exec("CREATE TABLE meals ("
               "id INTEGER PRIMARY KEY, "
               "name TEXT NOT NULL)");

    query.exec("CREATE TABLE mealcontents ("
               "id INTEGER PRIMARY KEY, "
               "mealid TEXT NOT NULL, "
               "foodid TEXT NOT NULL, "
               "weight INTEGER NOT NULL, "
               "FOREIGN KEY (mealid) REFERENCES meals (id), "
               "FOREIGN KEY (foodid) REFERENCES food (id))");
}

void SheetWindow::setupActions()
{
    // --- Sheet actions ---

    m_ui->newSheetAction->setIcon(QIcon::fromTheme("window-new"));
    m_ui->newSheetAction->setStatusTip(tr("Create a blank sheet in a new window"));
    m_ui->newSheetAction->setShortcut(QKeySequence::New);
    //connect.....

    m_ui->openAction->setIcon(QIcon::fromTheme("document-open"));
    m_ui->openAction->setStatusTip(tr("Open a sheet in current window"));
    m_ui->openAction->setShortcut(QKeySequence::Open);
    //connect.....

    // TODO: recent files actions

    m_ui->saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    m_ui->saveAsAction->setStatusTip(tr("Save the sheet as a new file"));
    m_ui->saveAsAction->setShortcut(QKeySequence::SaveAs);
    //connect.....

    m_ui->setFoodBankAction->setStatusTip(tr("Specify which food bank file to use"));

    m_ui->clearSheetAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_ui->clearSheetAction->setStatusTip(tr("Remove all meals from the sheet"));
    //connect.....

    m_ui->refreshAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_ui->refreshAction->setStatusTip(tr("Reload sheet from disk"));
    m_ui->refreshAction->setShortcut(QKeySequence::Refresh);
    //connect.....

    m_ui->closeAction->setIcon(QIcon::fromTheme("window-close"));
    m_ui->closeAction->setStatusTip(tr("Close this sheet"));
    m_ui->closeAction->setShortcut(QKeySequence::Close);
    //connect.....

    m_ui->quitAction->setIcon(QIcon::fromTheme("application-exit"));
    m_ui->quitAction->setStatusTip(tr("Close all sheets and exit"));
    m_ui->quitAction->setShortcut(QKeySequence::Quit);
    //connect.....


    // --- Meal actions ---

    m_ui->newMealAction->setIcon(QIcon::fromTheme("document-new"));
    m_ui->newMealAction->setStatusTip(tr("Add a new meal to the sheet"));
    m_ui->newMealAction->setShortcut(QKeySequence::fromString("ctrl+shift+n"));
    connect(m_ui->newMealAction, SIGNAL(triggered()), m_mealsModel, SLOT(onNewMeal()));
    connect(m_ui->newMealAction, SIGNAL(triggered()), m_mealToolBar, SLOT(onNewMeal()));

    m_ui->duplicateAction->setIcon(QIcon::fromTheme("edit-copy"));
    m_ui->duplicateAction->setStatusTip(tr("Duplicate the current meal"));

    m_ui->deleteAction->setIcon(QIcon::fromTheme("edit-delete"));
    m_ui->deleteAction->setStatusTip(tr("Remove selected meal from the sheet"));
//    ui->deleteAction->setEnabled(false);

    m_ui->clearMealAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_ui->clearMealAction->setStatusTip(tr("Remove all food items from the meal"));

    m_ui->addFoodAction->setIcon(QIcon::fromTheme("list-add"));
    m_ui->addFoodAction->setStatusTip(tr("Add food items to the selected meal"));
    m_ui->addFoodAction->setShortcut(QKeySequence::fromString("insert"));
//    ui->addFoodAction->setEnabled(false);

    m_ui->removeFoodAction->setIcon(QIcon::fromTheme("list-remove"));
    m_ui->removeFoodAction->setStatusTip(tr("Remove selected food items from meal"));
    m_ui->removeFoodAction->setShortcut(QKeySequence::fromString("delete"));
//    ui->removeFoodAction->setEnabled(false);
}

void SheetWindow::createToolBars()
{
    m_sheetToolBar = addToolBar(tr("&Sheet"));
    m_sheetToolBar->addAction(m_ui->newSheetAction);
    m_sheetToolBar->addAction(m_ui->openAction);
    m_sheetToolBar->addAction(m_ui->refreshAction);
//    m_sheetToolBar->setHidden(true);

    m_mealToolBar = new MealToolBar(m_mealsModel, this);
    addToolBar(m_mealToolBar);
    m_mealToolBar->addAction(m_ui->newMealAction);
    m_mealToolBar->addAction(m_ui->deleteAction);
    m_mealToolBar->addAction(m_ui->addFoodAction);
    m_mealToolBar->addAction(m_ui->removeFoodAction);
}

void SheetWindow::createModels()
{
    m_mealsModel = new MealsModel(QSqlDatabase::database(m_connectionName), this);
}

