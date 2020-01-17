#ifndef CURRENTMEALMODEL_H
#define CURRENTMEALMODEL_H

#include <QSqlQuery>
#include <QSqlQueryModel>


class CurrentMealModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    CurrentMealModel(const QString &dbConnectionName, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void populate(int mealId);

private:
    QString m_dbConnectionName;
    QSqlQuery m_query;
};

#endif // CURRENTMEALMODEL_H
