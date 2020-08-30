#include "IOHandler/SQLiteHandler.h"
#include <QDebug>


SQLiteHandler::SQLiteHandler(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../AAA.db");
    if(!db.open())
    {
        qDebug()<< db.lastError();
        return;
    }
    qDebug()<< "Successfully connected!";
    query = new QSqlQuery;


    setUpModels();
}

QSqlQueryModel *SQLiteHandler::getJobTypeModel() const
{
    return jobTypeModel;
}

bool SQLiteHandler::insertJobType(const QString &newJobType, const QString &oldJobType)
{
    if(oldJobType == newJobType || newJobType.isEmpty())
        return true;

    if(!oldJobType.isEmpty())
        removeJobType(oldJobType);

    query->prepare("INSERT INTO jobType (name) VALUES (?)");
    query->addBindValue(newJobType);
    auto result = query->exec();
    return result;
}

bool SQLiteHandler::removeJobType(const QString &jobtype)
{
    query->prepare("DELETE FROM jobtype WHERE name = ?");
    query->addBindValue(jobtype);
    return query->exec();
}

QSqlTableModel *SQLiteHandler::getMediaHouseModel() const
{
    return mediaHouseModel;
}

QStringList SQLiteHandler::getMediaHouseRow(const int id)
{
    query->prepare("SELECT * FROM mediaHouse WHERE id = ?");
    query->addBindValue(id);
    query->exec();

    QList<QVariant> list = query->boundValues().values();
    qDebug()<< list<< list.size();
    for(auto i = 0; i< list.size(); ++i)
            qDebug()<< list.at(i);

    QMapIterator<QString, QVariant> i(query->boundValues());
    while(i.hasNext())
    {
        i.next();
        qDebug()<< "Map"<< i.key() <<":"<< i.value();
    }

    QStringList strList;
    while(query->next())
    {
        strList
        << query->value("id").toString()
        << query->value("name").toString()
        << query->value("contactPerson").toString()
        << query->value("phone").toString()
        << query->value("email").toString()
        << query->value("address").toString()
        << query->value("city").toString()
        << query->value("state").toString()
        << query->value("GST").toString()
        << query->value("SC").toString();
    }

    return strList;

}

bool SQLiteHandler::insertMediaHouseRow(const QStringList &strList)
{
    // check if already exist or not
    query->prepare("SELECT id FROM mediaHouse WHERE id = ?");
    query->addBindValue(strList.at(0).toInt());
    if(!query->exec())
    {
        qDebug()<< query->lastError();
        exit(-1);
    }

    // If already exist then update
    if(query->next())
    {
        auto name = strList[1];
        auto address = strList[5];
        query->prepare("UPDATE mediaHouse SET name = :name, contactPerson = :contactPerson, phone = :phone, email = :email, address = :address, city = :city, state = :state, GST = :GST, SC = :SC WHERE id = :id");
        query->bindValue(":id", strList.at(0));
        query->bindValue(":name", strList.at(1));
        query->bindValue(":contactPerson", strList.at(2));
        query->bindValue(":phone", strList.at(3));
        query->bindValue(":email", strList.at(4));
        query->bindValue(":address", strList.at(5));
        query->bindValue(":city", strList.at(6));
        query->bindValue(":state", strList.at(7));
        query->bindValue(":GST", strList.at(8));
        query->bindValue(":SC", strList.at(9));
    }
    else // else do INSERT
    {
        query->prepare("INSERT INTO mediaHouse (name, contactPerson, phone, email, address, city, state, GST, SC) VALUES (:name, :contactPerson, :phone, :email, :address, :city, :state, :GST, :SC)");
        query->bindValue(":name", strList.at(1));
        query->bindValue(":contactPerson", strList.at(2));
        query->bindValue(":phone", strList.at(3));
        query->bindValue(":email", strList.at(4));
        query->bindValue(":address", strList.at(5));
        query->bindValue(":city", strList.at(6));
        query->bindValue(":state", strList.at(7));
        query->bindValue(":GST", strList.at(8));
        query->bindValue(":SC", strList.at(9));
    }
    if(auto result = query->exec())
        return result;
    else
        qDebug()<< query->lastError();
    exit(-1);
}

bool SQLiteHandler::removeMediHouseaRow(const int id)
{
    if(!query->exec(QString("DELETE FROM mediaHouse WHERE id = %0").arg(id)))
    {
        qDebug()<< query->lastError();
        return false;
    }
    return true;
}

QSqlTableModel *SQLiteHandler::getClietModel() const
{
    return clientsModel;
}

QStringList SQLiteHandler::getClientRow(const int id)
{
    query->prepare("SELECT * FROM clients WHERE id = ?");
    query->addBindValue(id);
    query->exec();

    QStringList strList;
    while(query->next())
    {
        strList
        << query->value("id").toString()
        << query->value("name").toString()
        << query->value("contactPerson").toString()
        << query->value("phone").toString()
        << query->value("email").toString()
        << query->value("address").toString()
        << query->value("city").toString()
        << query->value("state").toString()
        << query->value("GST").toString()
        << query->value("pincode").toString();
    }

    return strList;
}

bool SQLiteHandler::insertClientRow(const QStringList &strList)
{
    // check if already exist or not
    query->prepare("SELECT id FROM clients WHERE id = ?");
    query->addBindValue(strList.at(0).toInt());
    if(!query->exec())
    {
        qDebug()<< query->lastError();
        exit(-1);
    }

    // If already exist then update
    if(query->next())
    {
        auto name = strList[1];
        auto address = strList[5];
        query->prepare("UPDATE clients SET name = :name, contactPerson = :contactPerson, phone = :phone, email = :email, address = :address, city = :city, state = :state, GST = :GST, pincode = :pincode WHERE id = :id");
        query->bindValue(":id", strList.at(0));
        query->bindValue(":name", strList.at(1));
        query->bindValue(":contactPerson", strList.at(2));
        query->bindValue(":phone", strList.at(3));
        query->bindValue(":email", strList.at(4));
        query->bindValue(":address", strList.at(5));
        query->bindValue(":city", strList.at(6));
        query->bindValue(":state", strList.at(7));
        query->bindValue(":GST", strList.at(8));
        query->bindValue(":pincode", strList.at(9));
    }
    else // else do INSERT
    {
        query->prepare("INSERT INTO clients (name, contactPerson, phone, email, address, city, state, GST, SC) VALUES (:name, :contactPerson, :phone, :email, :address, :city, :state, :GST, :SC)");
        query->bindValue(":name", strList.at(1));
        query->bindValue(":contactPerson", strList.at(2));
        query->bindValue(":phone", strList.at(3));
        query->bindValue(":email", strList.at(4));
        query->bindValue(":address", strList.at(5));
        query->bindValue(":city", strList.at(6));
        query->bindValue(":state", strList.at(7));
        query->bindValue(":GST", strList.at(8));
        query->bindValue(":SC", strList.at(9));
    }
    if(auto result = query->exec())
        return result;
    else
        qDebug()<< query->lastError();
    exit(-1);
}

bool SQLiteHandler::removeClientRow(const int id)
{
    if(!query->exec(QString("DELETE FROM clients WHERE id = %0").arg(id)))
    {
        qDebug()<< query->lastError();
        return false;
    }
    return true;
}

void SQLiteHandler::setUpModels()
{
    jobTypeModel = new QSqlQueryModel;
    jobTypeModel->setQuery("SELECT name from jobType");
    jobTypeModel->setHeaderData(0, Qt::Horizontal, "JobType");

    mediaHouseModel = new QSqlTableModel;
    mediaHouseModel->setTable("mediaHouse");
    mediaHouseModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mediaHouseModel->select();
    mediaHouseModel->setHeaderData(0, Qt::Horizontal, "id");
    mediaHouseModel->setHeaderData(1, Qt::Horizontal, "Name");
    mediaHouseModel->setHeaderData(2, Qt::Horizontal, "Contact Person");
    mediaHouseModel->setHeaderData(3, Qt::Horizontal, "Phone");
    mediaHouseModel->setHeaderData(4, Qt::Horizontal, "Email");
    mediaHouseModel->setHeaderData(5, Qt::Horizontal, "Address");
    mediaHouseModel->setHeaderData(6, Qt::Horizontal, "City");
    mediaHouseModel->setHeaderData(7, Qt::Horizontal, "State");
    mediaHouseModel->setHeaderData(8, Qt::Horizontal, "GST");
    mediaHouseModel->setHeaderData(9, Qt::Horizontal, "SC");

    clientsModel = new QSqlTableModel;
    clientsModel->setTable("clients");
    clientsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    clientsModel->select();
    clientsModel->setHeaderData(0, Qt::Horizontal, "id");
    clientsModel->setHeaderData(1, Qt::Horizontal, "Name");
    clientsModel->setHeaderData(2, Qt::Horizontal, "Contact Person");
    clientsModel->setHeaderData(3, Qt::Horizontal, "Phone");
    clientsModel->setHeaderData(4, Qt::Horizontal, "Email");
    clientsModel->setHeaderData(5, Qt::Horizontal, "Address");
    clientsModel->setHeaderData(6, Qt::Horizontal, "City");
    clientsModel->setHeaderData(7, Qt::Horizontal, "State");
    clientsModel->setHeaderData(8, Qt::Horizontal, "GST");
    clientsModel->setHeaderData(9, Qt::Horizontal, "Pin Code");


}

SQLiteHandler::~SQLiteHandler()
{

}

