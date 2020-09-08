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
#include <QMessageBox>

class SQLiteHandler : public QObject
{
    Q_OBJECT

    QSqlDatabase db;
    QSqlQuery *query;

    QStringList *gstPerc;
    QSqlQueryModel *jobTypeModel;
    QSqlTableModel *mediaHouseModel;
    QSqlTableModel *clientsModel;
    QSqlTableModel *roModel;
    QSqlTableModel *paymentModel;
    QSqlTableModel *receiptModel;
    QSqlTableModel *mediaBillModel;
    QSqlQueryModel *generateBillModel;

    void setUpModels();
public:
    explicit SQLiteHandler(QObject *parent = nullptr);
    virtual ~SQLiteHandler();

    QSqlQueryModel *getJobTypeModel() const;
    bool insertJobType(const QString &newJobType, const QString &oldJobType = "");
    bool removeJobType(const QString &jobtype);
    int getJobTypeCode(const QString &jobTypeName);
    QStringList getJobTypeList();

    QSqlTableModel *getMediaHouseModel() const;
    QStringList getMediaHouseRow(const int id);
    bool insertMediaHouseRow(const QStringList &strList);
    bool removeMediHouseaRow(const int id);
    int getMediaHouseCode(const QString mediaHouseName);
    QStringList getMediaHouseList();

    QSqlTableModel *getClietModel() const;
    QStringList getClientRow(const int id);
    bool insertClientRow(const QStringList &strList);
    bool removeClientRow(const int id);
    int getClientCode(QString clientName);
    const QString getClientName(const int pcode);
    QStringList getClientList();

    QSqlTableModel *getRoModel() const;
    bool insertRoData(QStringList dataList);
    const QStringList getROStringList(const int id);
    int getNewRoNumber() const;
    int getNewRoCode() const;
    QStringList getRoList();

    QSqlTableModel *getPaymentModel() const;
    bool insertPayment(QStringList dataList);
    QStringList paymentStringList(int roNo);
    bool insertBlankRow(int rono);

    QSqlTableModel *getReceiptModel() const;
    bool insertReceipt(QStringList dataList);
    QStringList receiptStringList();

    QSqlTableModel *getMediaBill() const;


    QSqlQueryModel *getGenerateBillModel() const;
    QStringList getGenerateBillList(const int invno);
    bool insertGenerateBillList(QStringList strList);
    int getNewInvoiceCode();
    QStringList *getGstPerc() const;

signals:

};

#endif // SQLITEHANDLER_H
