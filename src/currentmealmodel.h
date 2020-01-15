#ifndef CURRENTMEALMODEL_H
#define CURRENTMEALMODEL_H

#include <QSqlQueryModel>


class CurrentMealModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    CurrentMealModel(const QString &dbConnectionName, QObject *parent = nullptr);

private:
    QString m_dbConnectionName;
};

#endif // CURRENTMEALMODEL_H
