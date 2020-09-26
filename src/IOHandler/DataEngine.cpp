#include "IOHandler/DataEngine.h"

QMap<QString, QStringList> *DataEngine::getClientMap() const
{
    return clientMap;
}

QMap<QString, QStringList> *DataEngine::getMediaHouseMap() const
{
    return mediaHouseMap;
}

DataEngine::DataEngine()
{
    jobTypeSets = new QSet<QString>({"PVR CINEMA", "WAVE CINEMA", "AMAR UJALA", "DANIK JAGRAN"});
    clientMap = new QMap<QString, QStringList>;
    mediaHouseMap = new QMap<QString, QStringList>;
    ROMap = new QMap<int, QStringList>;
    paymentMap = new QMap<int, QStringList>;
    receiptMap = new QMap<int, QStringList>;
    gBillMap = new QMap<int, QStringList>;
    mediaBillMap = new QMap<int, QStringList>;
}

DataEngine::~DataEngine()
{

}

QSet<QString> *DataEngine::getJobTypeSets() const
{
    return jobTypeSets;
}

void DataEngine::insertJobTypeData(const QString &newJobType, const QString &oldJobType)
{
    if(oldJobType == newJobType || newJobType.isEmpty())
        return;

    if(!oldJobType.isEmpty())
        jobTypeSets->remove(oldJobType);

    jobTypeSets->insert(newJobType);

}

void DataEngine::removeJobTypeData(const QString &jobType)
{
    jobTypeSets->remove(jobType);
}

void DataEngine::insertClientData(const QString &client, const QStringList &clientDetails)
{
    clientMap->insert(client, clientDetails);
}

void DataEngine::removeClientData(const QString &client)
{
    clientMap->remove(client);
}

void DataEngine::insertMediaHouseData(const QString &mediaHouse, const QStringList &clientDetails)
{
    mediaHouseMap->insert(mediaHouse, clientDetails);
}

void DataEngine::removeMediaHouseData(const QString &mediaHouse)
{
    mediaHouseMap->remove(mediaHouse);
}

QStringList DataEngine::mediaHouseStringList()
{
    QStringList strList("");
    for(auto mediaHouse=mediaHouseMap->begin(); mediaHouse!= mediaHouseMap->end(); ++mediaHouse)
        strList << mediaHouse.key();

    return strList;
}

void DataEngine::insertROData(const QStringList detailList)
{
    auto RONo = detailList[0].toInt();

    ROMap->insert(RONo, detailList);
}

QStringList DataEngine::roStringList()
{
    QStringList roString;
    for(auto ro: *ROMap)
        roString << ro.at(0);

    return roString;
}

QStringList DataEngine::roStringList(const int roNo)
{
    auto it = ROMap->find(roNo);
    if(it == ROMap->end())
        return {};
    return it.value();
}

void DataEngine::insertPaymentData(const QStringList &paymentDetail)
{
    auto RONo = paymentDetail.at(0).toInt();

    paymentMap->insert(RONo, paymentDetail);
}

void DataEngine::RemovePaymentData(const QString &roNo)
{
    paymentMap->remove(roNo.toInt());
}

QStringList DataEngine::paymentStringList(int roNo)
{
    auto idt = paymentMap->find(roNo);
    if(idt != paymentMap->end())
        return idt.value();
    return {};
}

void DataEngine::insertReceiptData(const QStringList &receiptDetail)
{
    auto RONo = receiptDetail.at(0).toInt();

    receiptMap->insert(RONo, receiptDetail);
}

void DataEngine::RemoveReceiptData(const QString &roNo)
{
    receiptMap->remove(roNo.toInt());
}

QStringList DataEngine::receiptStringList(int roNo)
{
    auto idt = receiptMap->find(roNo);
    if(idt != receiptMap->end())
        return idt.value();
    return {};
}

QMap<int, QStringList> *DataEngine::getGBillMap() const
{
    return gBillMap;
}

QMap<int, QStringList> *DataEngine::getMediaBillMap() const
{
    return mediaBillMap;
}

void DataEngine::insertMediaBillData(const QStringList &mediaBillDetail)
{
    auto RONo = mediaBillDetail.at(0).toInt();

    mediaBillMap->insert(RONo, mediaBillDetail);
}

void DataEngine::RemoveMediaBillData(const QString &roNo)
{
    mediaBillMap->remove(roNo.toInt());
}

QStringList DataEngine::mediaBillStringList(int roNo)
{
    auto idt = mediaBillMap->find(roNo);
    if(idt != mediaBillMap->end())
        return idt.value();
    return {};
}

void DataEngine::insertGBillData(const QStringList &gBillDetail)
{
    auto RONo = gBillDetail.at(0).toInt();

    gBillMap->insert(RONo, gBillDetail);
}

void DataEngine::RemoveGBillData(const QString &roNo)
{
    gBillMap->remove(roNo.toInt());
}

QStringList DataEngine::gBillStringList(int roNo)
{
    auto it = gBillMap->find(roNo);
    if(it != gBillMap->end())
        return  it.value();
    return {};
}

QStringList DataEngine::jobTypeStringList()
{
    QStringList strList("");
    for(auto jobType: *jobTypeSets)
        strList<< jobType;

    return strList;
}

QStringList DataEngine::clientStringList()
{
    QStringList strList;
    for(auto client= clientMap->begin(); client != clientMap->end(); ++client)
        strList<< client.key();

    return strList;
}

