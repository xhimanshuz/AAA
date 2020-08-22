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

QStringList DataEngine::jobTypeStringList()
{
    QStringList strList("");
    for(auto jobType: *jobTypeSets)
        strList<< jobType;

    return strList;
}

QStringList DataEngine::clientStringList()
{
    QStringList strList("");
    for(auto client= clientMap->begin(); client != clientMap->end(); ++client)
        strList<< client.key();

    return strList;
}

