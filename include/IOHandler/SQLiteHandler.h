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
#include "ROSqlTableModel.h"

class SQLiteHandler : public QObject
{
    Q_OBJECT

    QSqlDatabase db;
    QSqlDatabase locationDb;
    QSqlQuery *query;
    QSqlQuery *lquery;

    QStringList *gstPerc;
    QSqlQueryModel *jobTypeModel;
    QSqlTableModel *mediaHouseModel;
    QSqlTableModel *clientsModel;
    ROTableModel *roModel;
    QSqlTableModel *paymentModel;
    QSqlTableModel *receiptModel;
    QSqlTableModel *mediaBillModel;
    QSqlQueryModel *generateBillModel;

    void setUpModels();
    
    enum class ROIndex
    {
        MH_CODE,
        P_CODE,
        JOBT_CODE,
        INV_NO,
        RECPT_NO
    };
    
public:
    void reloadDB();
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
    bool updateNumberToRo(const QString &numbers, const int ro, ROIndex index);

    QSqlTableModel *getMediaPaymentModel() const;
    bool insertMediaPayment(QList<QStringList> dataList, int rono);
    QList<QStringList> getMediaPaymentStringListByRono(int roNo);
    int getNewMediaPaymentNumber();
    QStringList getMediaPaymentStringList(int mpId);

    QSqlTableModel *getPaymentReceiptModel() const;
    bool insertPaymentReceipt(QList<QStringList> dataList, int rono);
    QList<QStringList> getPaymentReceiptStringListByRO(int rono);
    QStringList getPaymentReceiptStringList(int prNo);
    int getNewPaymentReceiptNumber();

    QSqlTableModel *getMediaBill() const;
    QList<QStringList> getMediaBillList(int rono);
    bool insertMediaBill(QList<QStringList> dataList, int rono);


    QSqlQueryModel *getInvoiceModel() const;
    QStringList getInvoiceList(const int invno);
    bool insertInvoiceList(QStringList strList);
    int getNewInvoiceCode();
    QList<QStringList> getInvoiceListByRoNo(const int rono);
    QStringList *getGstPerc() const;


    bool setConfig(const QStringList &configList);
    const QStringList getConfigList() const;

    QStringList getStates() const;
    QStringList getCities(const QString state) const;
    QString getPinCode(QString city) const;
    QString getStateCode(QString state) const;

    const QString combineNumber(const QStringList &numberList);
signals:

};

#endif // SQLITEHANDLER_H
