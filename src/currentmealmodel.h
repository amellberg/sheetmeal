#ifndef CURRENTMEALMODEL_H
#define CURRENTMEALMODEL_H

#include <QSqlQueryModel>


class CurrentMealModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    CurrentMealModel(const QString &dbConnectionName, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QString m_dbConnectionName;
};

#endif // CURRENTMEALMODEL_H
