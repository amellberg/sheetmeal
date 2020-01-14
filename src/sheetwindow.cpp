#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryFile>
#include <QToolBar>
#include <QUuid>

#include "mealsmodel.h"
#include "sheetwindow.h"
#include "ui_sheetwindow.h"


// Ugly workaround for the meals model resetting the combo box selection.
// We write to this variable before renaming a meal, and read from it
// after the model has updated and reset the view.
static int tempMealIndex = 0;


SheetWindow::SheetWindow(QString sheetPath, QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::SheetWindow),
      lastMealIndex(0),
      restoreMealIndex(false),
      m_sheetPath(sheetPath)
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

//    connect(m_mealToolBar, &MealToolBar::mealNameChanged,
//            m_mealsModel, &MealsModel::onMealNameChanged);
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

void SheetWindow::onNewMeal()
{
    int numMeals = m_mealsModel->rowCount();
    if (numMeals == 1) {
        m_mealsComboBox->setEnabled(true);
        m_ui->renameMealAction->setEnabled(true);
        m_ui->deleteMealAction->setEnabled(true);
        m_ui->addFoodAction->setEnabled(true);
        m_ui->duplicateMealAction->setEnabled(true);
    }
    m_mealsComboBox->setCurrentIndex(numMeals - 1);
}

void SheetWindow::onRenameMeal()
{
    QString currentMealName = m_mealsComboBox->currentText();
    bool okToProceed;
    QString newMealName = QInputDialog::getText(
                this, tr("Rename Meal"), tr("Meal name:"),
                QLineEdit::Normal, currentMealName, &okToProceed);

    if (okToProceed && newMealName != currentMealName) {
        lastMealIndex = m_mealsComboBox->currentIndex();
        restoreMealIndex = true;
        emit mealNameChanged(m_mealsComboBox->currentIndex(), newMealName);
    }
}

void SheetWindow::onMealModelReset()
{
    if (restoreMealIndex) {
        m_mealsComboBox->setCurrentIndex(lastMealIndex);
        restoreMealIndex = false;
    }
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
    m_ui->newSheetAction->setStatusTip(m_ui->newSheetAction->toolTip());
    m_ui->newSheetAction->setShortcut(QKeySequence::New);
    //connect.....

    m_ui->openAction->setIcon(QIcon::fromTheme("document-open"));
    m_ui->openAction->setStatusTip(m_ui->openAction->toolTip());
    m_ui->openAction->setShortcut(QKeySequence::Open);
    //connect.....

    // TODO: recent files actions

    m_ui->saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    m_ui->saveAsAction->setStatusTip(m_ui->saveAsAction->toolTip());
    m_ui->saveAsAction->setShortcut(QKeySequence::SaveAs);
    //connect.....

    m_ui->clearSheetAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_ui->clearSheetAction->setStatusTip(m_ui->clearSheetAction->toolTip());
    //connect.....

    m_ui->refreshSheetAction->setIcon(QIcon::fromTheme("view-refresh"));
    m_ui->refreshSheetAction->setStatusTip(m_ui->refreshSheetAction->toolTip());
    m_ui->refreshSheetAction->setShortcut(QKeySequence::Refresh);
    //connect.....

    m_ui->closeSheetAction->setIcon(QIcon::fromTheme("window-close"));
    m_ui->closeSheetAction->setStatusTip(m_ui->closeSheetAction->toolTip());
    m_ui->closeSheetAction->setShortcut(QKeySequence::Close);
    //connect.....

    m_ui->quitAction->setIcon(QIcon::fromTheme("application-exit"));
    m_ui->quitAction->setStatusTip(m_ui->quitAction->toolTip());
    m_ui->quitAction->setShortcut(QKeySequence::Quit);
    //connect.....


    // --- Meal actions ---

    m_ui->newMealAction->setIcon(QIcon::fromTheme("document-new"));
    m_ui->newMealAction->setStatusTip(m_ui->newMealAction->toolTip());
    m_ui->newMealAction->setShortcut(QKeySequence::fromString("ctrl+shift+n"));
    connect(m_ui->newMealAction, &QAction::triggered, m_mealsModel, &MealsModel::onNewMeal);
    connect(m_ui->newMealAction, &QAction::triggered, this, &SheetWindow::onNewMeal);

    m_ui->duplicateMealAction->setIcon(QIcon::fromTheme("edit-copy"));
    m_ui->duplicateMealAction->setStatusTip(m_ui->duplicateMealAction->toolTip());

    m_ui->renameMealAction->setIcon(QIcon::fromTheme("accessories-text-editor"));
    m_ui->renameMealAction->setStatusTip(m_ui->renameMealAction->toolTip());
    connect(m_ui->renameMealAction, &QAction::triggered, this, &SheetWindow::onRenameMeal);
    connect(this, &SheetWindow::mealNameChanged, m_mealsModel, &MealsModel::onMealNameChanged);

    m_ui->deleteMealAction->setIcon(QIcon::fromTheme("edit-delete"));
    m_ui->deleteMealAction->setStatusTip(m_ui->deleteMealAction->toolTip());
//    ui->deleteAction->setEnabled(false);

    m_ui->clearMealAction->setIcon(QIcon::fromTheme("edit-clear"));
    m_ui->clearMealAction->setStatusTip(m_ui->clearMealAction->toolTip());

    m_ui->addFoodAction->setIcon(QIcon::fromTheme("list-add"));
    m_ui->addFoodAction->setStatusTip(m_ui->addFoodAction->toolTip());
    m_ui->addFoodAction->setShortcut(QKeySequence::fromString("insert"));
//    ui->addFoodAction->setEnabled(false);

    m_ui->removeFoodAction->setIcon(QIcon::fromTheme("list-remove"));
    m_ui->removeFoodAction->setStatusTip(m_ui->removeFoodAction->toolTip());
    m_ui->removeFoodAction->setShortcut(QKeySequence::fromString("delete"));
//    ui->removeFoodAction->setEnabled(false);
}

// Note to self: there is some inconsistency in not defining the toolbars
// through the UI-file like the rest of the widgets.
void SheetWindow::createToolBars()
{
    m_sheetToolBar = addToolBar(tr("&Sheet"));
    m_sheetToolBar->addAction(m_ui->newSheetAction);
    m_sheetToolBar->addAction(m_ui->openAction);
    m_sheetToolBar->addAction(m_ui->refreshSheetAction);
//    m_sheetToolBar->setHidden(true);

    m_mealsComboBox = new QComboBox;
    m_mealsComboBox->setModel(m_mealsModel);
    m_mealsComboBox->setModelColumn(1);  // Display 'name' column
    m_mealsComboBox->setMinimumWidth(125);
    m_mealsComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_mealsComboBox->setEnabled(false);

    m_mealsToolBar = addToolBar(tr("&Meal"));
    m_mealsToolBar->addWidget(m_mealsComboBox);
    m_mealsToolBar->addAction(m_ui->newMealAction);
    m_mealsToolBar->addAction(m_ui->deleteMealAction);
    m_mealsToolBar->addAction(m_ui->renameMealAction);
    m_mealsToolBar->addAction(m_ui->addFoodAction);
    m_mealsToolBar->addAction(m_ui->removeFoodAction);
}

void SheetWindow::createModels()
{
    m_mealsModel = new MealsModel(QSqlDatabase::database(m_connectionName), this);
    connect(m_mealsModel, &QAbstractItemModel::modelReset,
            this, &SheetWindow::onMealModelReset);
}
