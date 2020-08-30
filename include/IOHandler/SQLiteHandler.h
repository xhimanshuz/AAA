#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlResult>
#include <QVariant>
#include <QList>
#include <QMapIterator>

class SQLiteHandler : public QObject
{
    Q_OBJECT

    QSqlDatabase db;
    QSqlQuery *query;

    QSqlQueryModel *jobTypeModel;
    QSqlTableModel *mediaHouseModel;
    QSqlTableModel *clientsModel;

    void setUpModels();
public:
    explicit SQLiteHandler(QObject *parent = nullptr);
    virtual ~SQLiteHandler();

    QSqlQueryModel *getJobTypeModel() const;
    bool insertJobType(const QString &newJobType, const QString &oldJobType = "");
    bool removeJobType(const QString &jobtype);

    QSqlTableModel *getMediaHouseModel() const;
    QStringList getMediaHouseRow(const int id);
    bool insertMediaHouseRow(const QStringList &strList);
    bool removeMediHouseaRow(const int id);

    QSqlTableModel *getClietModel() const;
    QStringList getClientRow(const int id);
    bool insertClientRow(const QStringList &strList);
    bool removeClientRow(const int id);

signals:

};

#endif // SQLITEHANDLER_H
