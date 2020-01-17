#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlIndex>
#include <QSqlQuery>
#include <QTemporaryFile>
#include <QToolBar>
#include <QUuid>

#include "sheetwindow.h"
#include "ui_sheetwindow.h"
#include "mealsmodel.h"
#include "currentmealmodel.h"
#include "mealtable.h"
#include "totalstable.h"


SheetWindow::SheetWindow(QString sheetPath, QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::SheetWindow),
      m_storedMealIndex(0),
      m_restoreMealIndex(false),
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

    m_ui->setupUi(this);
    createModels();
    createToolBars();
    createMainLayout();
    setupActions();

    if (sheetPath.isEmpty()) {
        initializeSheet();
    } else {
        onMealSwitch(0);
    }

//    onMealSwitch(0);
    sheetIsNonEmpty();
    resize(700, 500);
}

SheetWindow::~SheetWindow()
{
    {
        auto db = QSqlDatabase::database(m_dbConnectionName);
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(m_dbConnectionName);

    delete m_ui;
}

void SheetWindow::onDeleteMeal()
{
    auto answer = QMessageBox::question(
                this,
                tr("Delete Meal"),
                tr("Permanently delete meal ") + m_mealsComboBox->currentText() + "?",
                QMessageBox::Yes | QMessageBox::Cancel,
                QMessageBox::Yes);
    if (answer == QMessageBox::Cancel) {
        return;
    }

    int numMeals = m_mealsComboBox->count();
    if (numMeals == 1) {
        sheetIsEmpty();
    } else if (numMeals > 1) {
        m_storedMealIndex = m_mealsComboBox->currentIndex();
        if (m_storedMealIndex == numMeals - 1) {
            m_storedMealIndex--;
        }
        m_restoreMealIndex = true;
    }
    emit mealDeleted(m_mealsComboBox->currentIndex());
}

void SheetWindow::onNewMeal()
{
    int numMeals = m_mealsModel->rowCount();
    if (numMeals == 1) {
        sheetIsNonEmpty();
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
        m_storedMealIndex = m_mealsComboBox->currentIndex();
        m_restoreMealIndex = true;
        emit mealNameChanged(m_mealsComboBox->currentIndex(), newMealName);
    }
}

void SheetWindow::onMealSwitch(int index)
{
    int mealId = m_mealsModel->record(index).value("id").toInt();
    emit mealSwitched(mealId);
}

void SheetWindow::onMealsModelReset()
{
    if (m_restoreMealIndex) {
        m_mealsComboBox->setCurrentIndex(m_storedMealIndex);
        m_restoreMealIndex = false;
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
    m_dbConnectionName = QUuid::createUuid().toString();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_dbConnectionName);
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
    QSqlQuery query(QSqlDatabase::database(m_dbConnectionName));
    query.exec("PRAGMA foreign_keys = true");

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
               "mealid INTEGER NOT NULL, "
               "foodid INTEGER NOT NULL, "
               "weight INTEGER NOT NULL, "
               "FOREIGN KEY (mealid) REFERENCES meals (id) ON DELETE CASCADE, "
               "FOREIGN KEY (foodid) REFERENCES fooditems (id) ON DELETE CASCADE)");

    /*
    // Some test data
    query.exec("INSERT INTO fooditems (name, carbs, fat, protein, kcal) "
               "VALUES ('Mellanmjölk', 34.2, 3.3, 11, 194.5)");
    query.exec("INSERT INTO fooditems (name, carbs, fat, protein, kcal) "
               "VALUES ('Knäckebröd', 49.3, 1.3, 5.3, 326.1)");
    query.exec("INSERT INTO meals (name) VALUES ('Breakfast')");
    query.exec("INSERT INTO meals (name) VALUES ('Lunch')");
    query.exec("INSERT INTO mealcontents (mealid, foodid, weight) VALUES (1, 1, 175)");
    query.exec("INSERT INTO mealcontents (mealid, foodid, weight) VALUES (1, 2, 45)");
    query.exec("INSERT INTO mealcontents (mealid, foodid, weight) VALUES (2, 1, 150)");

    query.exec("DELETE FROM meals WHERE id = 1");
    qDebug() << query.lastError();
    */
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
    connect(m_ui->deleteMealAction, &QAction::triggered, this, &SheetWindow::onDeleteMeal);
    connect(this, &SheetWindow::mealDeleted, m_mealsModel, &MealsModel::onDeleteMeal);

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
    connect(m_mealsComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onMealSwitch(int)));

    m_mealsToolBar = addToolBar(tr("&Meal"));
    m_mealsToolBar->addWidget(m_mealsComboBox);
    m_mealsToolBar->addAction(m_ui->newMealAction);
    m_mealsToolBar->addAction(m_ui->deleteMealAction);
    m_mealsToolBar->addAction(m_ui->renameMealAction);
    m_mealsToolBar->addAction(m_ui->addFoodAction);
    m_mealsToolBar->addAction(m_ui->removeFoodAction);
}

void SheetWindow::createMainLayout()
{
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    setCentralWidget(splitter);
    m_mealTable = new MealTable(m_currentMealModel);
    m_totalsTable = new TotalsTable;  // TODO: pass model
    splitter->addWidget(m_mealTable);
    splitter->addWidget(m_totalsTable);
}

void SheetWindow::createModels()
{
    m_mealsModel = new MealsModel(QSqlDatabase::database(m_dbConnectionName), this);
    connect(m_mealsModel, &QAbstractItemModel::modelReset,
            this, &SheetWindow::onMealsModelReset);

    m_currentMealModel = new CurrentMealModel(m_dbConnectionName, this);
    connect(this, SIGNAL(mealSwitched(int)), m_currentMealModel, SLOT(populate(int)));
}

void SheetWindow::sheetIsEmpty()
{
    m_mealsComboBox->setEnabled(false);
    m_ui->renameMealAction->setEnabled(false);
    m_ui->deleteMealAction->setEnabled(false);
    m_ui->addFoodAction->setEnabled(false);
    m_ui->removeFoodAction->setEnabled(false);
    m_ui->duplicateMealAction->setEnabled(false);
    m_ui->clearMealAction->setEnabled(false);
}

void SheetWindow::sheetIsNonEmpty()
{
    m_mealsComboBox->setEnabled(true);
    m_ui->renameMealAction->setEnabled(true);
    m_ui->deleteMealAction->setEnabled(true);
    m_ui->addFoodAction->setEnabled(true);
    m_ui->duplicateMealAction->setEnabled(true);
}
