#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <QObject>
#include <QSet>
#include <QStringList>
#include <QMap>

class DataEngine
{
    QSet<QString> *jobTypeSets;
    QMap<QString, QStringList> *clientMap;      // { clientName, contact person, phone, emailID, address, city, state, gstinno., pincode }
    QMap<QString, QStringList> *mediaHouseMap;  // { clientName, contact person, phone, emailID, address, city, state, gstinno., pincode }
public:

    DataEngine();
    ~DataEngine();

    // JOB TYPE
    QSet<QString> *getJobTypeSets() const;
    void insertJobTypeData(const QString &newJobType, const QString &oldJobType = "" );
    void removeJobTypeData(const QString &jobType);
    QStringList jobTypeStringList();

    // CLIENT
    QMap<QString, QStringList> *getClientMap() const;
    void insertClientData(const QString &client, const QStringList &clientDetails);
    void removeClientData(const QString &client);
    QStringList clientStringList();

    // MEDIA HOUSE
    QMap<QString, QStringList> *getMediaHouseMap() const;
    void insertMediaHouseData(const QString &mediaHouse, const QStringList &clientDetails);
    void removeMediaHouseData(const QString &mediaHouse);
    QStringList mediaHouseStringList();

    // RO MAP
    QMap<int, QStringList> *ROMap;
    void insertROData(const QStringList detailList);
    QStringList roStringList(const int roNo);
    QStringList roStringList();

    // PAYMENT MAP
    QMap<int, QStringList> *paymentMap;
    void insertPaymentData(const QStringList &paymentDetail);
    void RemovePaymentData(const QString &roNo);
    QStringList paymentStringList(int roNo);

    // RECEIPT MAP
    QMap<int, QStringList> *receiptMap;
    void insertReceiptData(const QStringList &receiptDetail);
    void RemoveReceiptData(const QString &roNo);
    QStringList receiptStringList(int roNo);

    // GENERATE BILL MAP
    QMap<int, QStringList> *gBillMap;
    void insertGBillData(const QStringList &gBillDetail);
    void RemoveGBillData(const QString &roNo);
    QStringList gBillStringList(int roNo);
    QMap<int, QStringList> *getGBillMap() const;

    QMap<int, QStringList> *mediaBillMap;
    void insertMediaBillData(const QStringList &mediaBillDetail);
    void RemoveMediaBillData(const QString &roNo);
    QStringList mediaBillStringList(int roNo);
    QMap<int, QStringList> *getMediaBillMap() const;
};

#endif // DATAENGINE_H
