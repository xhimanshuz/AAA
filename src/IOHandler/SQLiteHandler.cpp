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

int SQLiteHandler::getJobTypeCode(const QString &jobTypeName)
{
    query->exec(QString("SELECT id FROM jobType WHERE name = '%0'").arg(jobTypeName));
    if(!query->next())
    {
        qDebug()<<query->lastError();
        return -1;
    }
    auto id = query->value(0).toInt();
    return id;
}

QStringList SQLiteHandler::getJobTypeList()
{
    query->exec("SELECT name FROM jobType");
    QStringList strList;
    strList << "";
    while(query->next())
        strList << query->value(0).toString();

    return strList;
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

int SQLiteHandler::getMediaHouseCode(const QString mediaHouseName)
{
    query->exec(QString("SELECT id FROM mediaHouse WHERE name = '%0'").arg(mediaHouseName));
    if(!query->next())
    {
        qDebug()<< query->lastError();
        return -1;
    }
    auto id = query->value(0).toInt();
    return id;
}

QStringList SQLiteHandler::getMediaHouseList()
{
    query->exec("SELECT name FROM mediaHouse");
    QStringList strList;
    strList << "";
    while(query->next())
        strList << query->value(0).toString();

    return strList;
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

int SQLiteHandler::getClientCode(QString clientName)
{
    query->exec(QString("SELECT id FROM clients WHERE name = '%0'").arg(clientName));
    if(!query->next())
    {
        qDebug()<< query->lastError();
        return -1;
    }
    auto id = query->value(0).toInt();
    return id;
}

const QString SQLiteHandler::getClientName(const int pcode)
{
    query->exec(QString("SELECT name FROM clients WHERE id = %0").arg(pcode));
    if(!query->next())
    {
        qDebug()<< query->lastError();
        return "";
    }
    auto name = query->value(0).toString();
    return name;
}

QStringList SQLiteHandler::getClientList()
{
    query->exec("SELECT name FROM clients");
    QStringList strList;
    strList << "";
    while(query->next())
        strList << query->value(0).toString();

    return strList;
}

QSqlTableModel *SQLiteHandler::getRoModel() const
{
    return roModel;
}

bool SQLiteHandler::insertRoData(QStringList dataList)
{
    query->exec(QString("SELECT number FROM ro WHERE number = %0").arg(dataList.at(1)));
    if(!query->next())
    {
        query->prepare("INSERT INTO ro (code,number,date,mhcode,mhname,pcode,pname,jobtypecode,jobtypename,caption,editCentre,doPubtel,sizeduration,totalsizeduration,guarantedpos,premium,strPre,rate,strRate,amount,netAmount,remarks,billAmount,invno,payamount,recptno,recptamount,mbamount,ratecgst,amountcgst,ratesgst,amountsgst,rateigst,amountigst,finalamount,hsncode) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        query->bindValue(0, dataList.at(0));
        for(auto i=2; i< 36; i++)
        {
            query->bindValue(i, dataList.at(i));
        }
    }
    else
    {
        query->prepare("UPDATE ro SET code = :code, number = :number, date = :date, mhcode = :mhcode, mhname = :mhname, pcode = :pcode, pname = :pname, jobtypecode = :jobtypecode, jobtypename = :jobtypename, caption = :caption, editCentre = :editCentre, doPubtel = :doPubtel, sizeduration = :sizeduration, totalsizeduration = :totalsizeduration, guarantedpos = :guarantedpos, premium = :premium, strPre = :strPre, rate = :rate, strRate = :strRate, amount = :amount, netAmount = :netAmount, remarks = :remarks, billAmount = :billAmount, invno = :invno, payamount = :payamount, recptno = :recptno, recptamount = :recptamount, mbamount = :mbamount, ratecgst = :ratecgst, amountcgst = :amountcgst, ratesgst = :ratesgst, amountsgst = :amountsgst, rateigst = :rateigst, amountigst = :amountigst, finalamount = :finalamount, hsncode = :hsncode WHERE code = ?");

        for(auto i = 0; i< 36; i++)
        {
            query->bindValue(i, dataList.at(i));
        }
        query->bindValue(36, dataList.at(1));
        auto s = query->lastQuery();
    }
    if(!query->exec())
    {
        qDebug()<< query->lastError();
        return false;
    }
    return true;

}

const QStringList SQLiteHandler::getROStringList(const int id)
{
    query->exec(QString("SELECT * FROM ro WHERE number = %0").arg(id));
    if(!query->next())
    {
        qDebug()<< query->lastError();
        return {};
    }

    QStringList strList;
    for(auto i=0; i<36; i++)
        strList << query->value(i).toString();

    return strList;
}

int SQLiteHandler::getNewRoNumber() const
{
    query->exec("SELECT MAX(number) FROM ro");
    if(query->next())
        return query->value(0).toInt()+1;
    return -1;
}

int SQLiteHandler::getNewRoCode() const
{
    query->exec("SELECT MAX(code) FROM ro");
    if(query->next())
        return query->value(0).toInt()+1;
    qDebug()<< query->lastError();
    return -1;
}

QStringList SQLiteHandler::getRoList()
{
    query->exec("SELECT number FROM ro");
    QStringList strList;
    strList << "";
    while(query->next())
        strList << query->value(0).toString();

    return strList;
}

QSqlTableModel *SQLiteHandler::getPaymentModel() const
{
    return paymentModel;
}

bool SQLiteHandler::insertPayment(QList<QStringList> dataList, int rono)
{
    if(!query->exec(QString("DELETE FROM payment WHERE rono = %0;").arg(rono)))
    {
        qDebug()<<"Error in deleting in Receipt: "<< query->lastError();
        return false;
    }

    for(auto strList: dataList)
    {
        query->prepare("INSERT INTO payment (date, amount, mode, chequeNo, bankname, rono) VALUES (:date, :amount, :mode, :chequeNo, :bankname, :rono);");
        query->bindValue(":date", strList.at(0));
        query->bindValue(":amount", strList.at(1).toDouble());
        query->bindValue(":mode", strList.at(2));
        query->bindValue(":chequeNo", strList.at(3));
        query->bindValue(":bankname", strList.at(4));
        query->bindValue(":rono", rono);
        if(!query->exec())
        {
            qDebug()<<"Error in Inserting values in payment " << query->lastError();
            return false;
        }
    }
    return true;
}

QList<QStringList> SQLiteHandler::getPaymentStringList(int rono)
{
    query->exec(QString("SELECT date, amount, mode, chequeNo, bankname FROM payment WHERE rono = %0;").arg(rono));

    QList<QStringList> list;
    while(query->next())
    {
        QStringList strList;
        strList << query->value(0).toString() << QString::number(query->value(1).toDouble()) << query->value(2).toString()<< query->value(3).toString()<< query->value(4).toString();
        list.append(strList);
    }

    return list;
}

int SQLiteHandler::getNewPaymentNumber()
{
    query->exec("SELECT MAX(id) FROM payment;");

    if(!query->next())
        return -1;

    auto number = query->value(0).toInt();
    return number+1;
}


QSqlTableModel *SQLiteHandler::getReceiptModel() const
{
    return receiptModel;
}

bool SQLiteHandler::insertReceipt(QList<QStringList> dataList, int rono)
{
    if(!query->exec(QString("DELETE FROM receipt WHERE rono = %0;").arg(rono)))
    {
        qDebug()<<"Error in deleting in Receipt: "<< query->lastError();
        return false;
    }

    for(auto strList: dataList)
    {
        query->prepare("INSERT INTO receipt (rcptdate, rcptamount, paymode, chqno, bankname, remark, rono) VALUES (:date, :amount, :paymode, :chqno, :bankname, :remark, :rono);");
        query->bindValue(":date", strList.at(0));
        query->bindValue(":amount", strList.at(1).toDouble());
        query->bindValue(":paymode", strList.at(2));
        query->bindValue(":chqno", strList.at(3));
        query->bindValue(":bankname", strList.at(4));
        query->bindValue(":remark", strList.at(5));
        query->bindValue(":rono", rono);
        if(!query->exec())
        {
            qDebug()<<"Error in Inserting values in receipt " << query->lastError();
            return false;
        }
    }
    return true;

}

QList<QStringList> SQLiteHandler::getReceiptStringList(int rono)
{
    query->exec(QString("SELECT rcptdate, rcptamount, paymode, chqno, bankname, remark  FROM receipt WHERE rono = %0;").arg(rono));

    QList<QStringList> list;
    while(query->next())
    {
        QStringList strList;
        strList << query->value(0).toString() << QString::number(query->value(1).toDouble()) << query->value(2).toString() << query->value(3).toString()<< query->value(4).toString()
                   << query->value(5).toString();
        list.append(strList);
    }

    return list;
}

int SQLiteHandler::getNewReceiptNumber()
{
    query->exec("SELECT MAX(number) FROM receipt;");
    if(!query->next())
        return -1;
    auto number = query->value(0).toInt();
    return number+1;
}

QSqlTableModel *SQLiteHandler::getMediaBill() const
{
    return mediaBillModel;
}

QList<QStringList> SQLiteHandler::getMediaBillList(int rono)
{
    query->exec(QString("SELECT id, date, amount FROM mediaBill WHERE rono = %0").arg(rono));

    QList<QStringList> list;
    while(query->next())
    {
        QStringList strList;
        strList << query->value(0).toString() << query->value(1).toString() << QString::number(query->value(2).toDouble());
        list.append(strList);
    }

    return list;
}

bool SQLiteHandler::insertMediaBill(QList<QStringList> dataList, int rono)
{
    if(!query->exec(QString("DELETE FROM mediaBill WHERE rono = %0;").arg(rono)))
    {
        qDebug()<<"Error in deleting in Receipt: "<< query->lastError();
        return false;
    }

    for(auto strList: dataList)
    {
        query->prepare("INSERT INTO mediaBill (id, date, amount, rono) VALUES (:id, :date, :amount, :rono);");
        query->bindValue(":id", strList.at(0));
        query->bindValue(":date", strList.at(1));
        query->bindValue(":amount", strList.at(2).toDouble());
        query->bindValue(":rono", rono);
        if(!query->exec())
        {
            qDebug()<<"Error in Inserting values in mediaBill " << query->lastError();
            return false;
        }
    }
    return true;
}


QSqlQueryModel *SQLiteHandler::getGenerateBillModel() const
{
    return generateBillModel;
}

QStringList SQLiteHandler::getGenerateBillList(const int invno)
{
    query->exec(QString("SELECT rono, number, date, pcode, gramount, disrate, disamount, npamount, ratecgst, amountcgst, ratesgst, amountsgst, rateigst, amountigst, finalamount, invoiceremark, totalsizeduration FROM invoice WHERE number = %0").arg(invno));
    if(!query->next())
    {
        qDebug()<<"Error for invoice id: "<< invno<< query->lastError();
        return {};
    }

//    auto gramount = query->value(4).toDouble();
    auto gramount = QString::number(query->value(4).toDouble());
    auto disrate = QString::number(query->value(5).toDouble());
    auto disamount = QString::number(query->value(6).toDouble());
    auto npamount = QString::number(query->value(7).toDouble());
    auto ratecgst = QString::number(query->value(8).toDouble());
    auto amountcgst = QString::number(query->value(9).toDouble());
    auto ratesgst = QString::number(query->value(10).toDouble());
    auto amountsgst = QString::number(query->value(11).toDouble());
    auto rateigst = QString::number(query->value(12).toDouble());
    auto amountigst = QString::number(query->value(13).toDouble());
    auto finalamount = QString::number(query->value(14).toDouble());

    QStringList strList;
    strList << query->value(0).toString()
            << query->value(1).toString()
            << query->value(2).toString()
            << getClientName(query->value(3).toInt())
            << gramount
            << disrate
            << disamount
            << npamount
            << ratecgst
            << amountcgst
            << ratesgst
            << amountsgst
            << rateigst
            << amountigst
            << finalamount
            << query->value(15).toString()
            << query->value(16).toString();

    return strList;

}

bool SQLiteHandler::insertGenerateBillList(QStringList strList)
{
    query->exec(QString("SELECT number FROM invoice WHERE number = %0").arg(strList.at(1).toInt()));
    if(!query->next())
    {
        qDebug() << "[!] "<<strList.at(1).toInt()<<" Invoice number doesn't exist, so, Inserting new one!" << query->lastError();
        query->prepare("INSERT INTO invoice ('rono','date', 'pcode', 'gramount', 'disrate','disamount', 'npamount', 'ratecgst','amountcgst','ratesgst','amountsgst','rateigst','amountigst','finalamount', 'invoiceremark') VALUES (:rono,:date,:pcode,:gramount,:disrate,:disamount,:npamount,:ratecgst,:amountcgst,:ratesgst,:amountsgst,:rateigst,:amountigst,:finalamount,:invoiceremark)");
        query->bindValue(":rono", strList.at(0).toInt());
        query->bindValue(":date", strList.at(2));
        query->bindValue(":pcode", strList.at(3).toInt());
        query->bindValue(":gramount", strList.at(4).toDouble());
        query->bindValue(":disrate", strList.at(5).toDouble());
        query->bindValue(":disamount", strList.at(6).toDouble());
        query->bindValue(":npamount", strList.at(7).toDouble());
        query->bindValue(":ratecgst", strList.at(8).toDouble());
        query->bindValue(":amountcgst", strList.at(9).toDouble());
        query->bindValue(":ratesgst", strList.at(10).toDouble());
        query->bindValue(":amountsgst", strList.at(11).toDouble());
        query->bindValue(":rateigst", strList.at(12).toDouble());
        query->bindValue(":amountigst", strList.at(13).toDouble());
        query->bindValue(":finalamount", strList.at(14).toDouble());
        query->bindValue(":invoiceremark", strList.at(15));
    }
    else
    {
        qDebug()<<"[!] "<< strList.at(1).toInt()<<" Invoice number doesn't exist, so, Updating Existing one!";
        query->prepare("UPDATE invoice SET rono=:rono, date=:date, pcode=:pcode, gramount=:gramount, disrate=:disrate, disamount=:disamount, npamount=:npamount, ratecgst=:ratecgst, amountcgst=:amountcgst, ratesgst=:ratesgst, amountsgst=:amountsgst, rateigst=:rateigst, amountigst=:amountigst, finalamount=:finalamount, invoiceremark=:invoiceremark  WHERE number = :number");
        query->bindValue(":rono", strList.at(0).toInt());
        query->bindValue(":date", strList.at(2));
        query->bindValue(":pcode", strList.at(3).toInt());
        query->bindValue(":gramount", strList.at(4).toDouble());
        query->bindValue(":disrate", strList.at(5).toDouble());
        query->bindValue(":disamount", strList.at(6).toDouble());
        query->bindValue(":npamount", strList.at(7).toDouble());
        query->bindValue(":ratecgst", strList.at(8).toDouble());
        query->bindValue(":amountcgst", strList.at(9).toDouble());
        query->bindValue(":ratesgst", strList.at(10).toDouble());
        query->bindValue(":amountsgst", strList.at(11).toDouble());
        query->bindValue(":rateigst", strList.at(12).toDouble());
        query->bindValue(":amountigst", strList.at(13).toDouble());
        query->bindValue(":finalamount", strList.at(14).toDouble());
        query->bindValue(":invoiceremark", strList.at(15));
        query->bindValue(":number", strList.at(1).toInt());
//        query->exec();
    }
    if(!query->exec())
    {
        QMessageBox msg(QMessageBox::Critical, "Error Occured",QString("Error in Updating/Inserting "+query->lastError().text()), QMessageBox::Close);
        msg.exec();
        qDebug()<<"Error in Updating/Inserting "<< query->lastError();
        return false;
    }
    qDebug()<<"[!] Trasactoion Sucessfully with Invoice Number: "<< strList.at(1).toInt();

    query->prepare("UPDATE ro SET invno = :invno WHERE number = :number");
    query->bindValue(":invno", strList.at(1).toInt());
    query->bindValue(":number", strList.at(0).toInt());
    if(!query->exec())
    {
        QMessageBox msg(QMessageBox::Critical, "Error in Updating RO",QString(query->lastError().text()), QMessageBox::Close);
        msg.exec();
        qDebug()<<"Error in UPDATING RO "<<strList.at(0).toInt();
        return false;
    }
    qDebug()<<"[!] RO Number: "<< strList.at(0).toInt()<<" update with invoice number: "<< strList.at(1).toInt();
    QMessageBox msg(QMessageBox::Information, "Transaction Sucessfull",QString("RO Number:<b> %0</b> update with invoice number: <b>%1</b>").arg(QString::number(strList.at(0).toInt())).arg(QString::number(strList.at(1).toInt())), QMessageBox::Close);
    msg.exec();
    return true;
}

int SQLiteHandler::getNewInvoiceCode()
{
    query->exec("SELECT MAX(number) FROM invoice");
    if(query->next())
        return query->value(0).toInt()+1;
    qDebug()<< query->lastError();
    return -1;
}

QStringList *SQLiteHandler::getGstPerc() const
{
    return gstPerc;
}

void SQLiteHandler::setUpModels()
{
    gstPerc = new QStringList(QStringList()<< "2.5"<<"5.0"<<"6.0"<<"12.0"<<"9.0"<<"18.0"<<"14.0"<<"28.0");

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

    roModel = new QSqlTableModel;
    roModel->setTable("ro");
    roModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    roModel->select();
    roModel->setHeaderData(0, Qt::Horizontal, "Code");
    roModel->setHeaderData(1, Qt::Horizontal, "Ro No.");
    roModel->setHeaderData(2, Qt::Horizontal, "date");
    roModel->setHeaderData(3, Qt::Horizontal, "M.H. Code");
    roModel->setHeaderData(4, Qt::Horizontal, "M.H. Name");
    roModel->setHeaderData(5, Qt::Horizontal, "Client Code");
    roModel->setHeaderData(6, Qt::Horizontal, "Client Name.");
    roModel->setHeaderData(7, Qt::Horizontal, "Job Type ID");
    roModel->setHeaderData(8, Qt::Horizontal, "Job Type");
    roModel->setHeaderData(9, Qt::Horizontal, "Caption");
    roModel->setHeaderData(10, Qt::Horizontal, "Edit Centre");
    roModel->setHeaderData(11, Qt::Horizontal, "Do Pub Tel");
    roModel->setHeaderData(12, Qt::Horizontal, "Size Duration");
    roModel->setHeaderData(13, Qt::Horizontal, "Total Size Duration");
    roModel->setHeaderData(14, Qt::Horizontal, "Guaranteed Position");
    roModel->setHeaderData(15, Qt::Horizontal, "Premium");
    roModel->setHeaderData(16, Qt::Horizontal, "Str. Premium");
    roModel->setHeaderData(17, Qt::Horizontal, "Rate");
    roModel->setHeaderData(18, Qt::Horizontal, "Str. Rate");
    roModel->setHeaderData(19, Qt::Horizontal, "Amount");
    roModel->setHeaderData(20, Qt::Horizontal, "Net Amount");
    roModel->setHeaderData(21, Qt::Horizontal, "Remarks");
    roModel->setHeaderData(22, Qt::Horizontal, "Bill Amount");
    roModel->setHeaderData(23, Qt::Horizontal, "Invoice No.");
    roModel->setHeaderData(24, Qt::Horizontal, "Payment");
    roModel->setHeaderData(25, Qt::Horizontal, "Receipt No.");
    roModel->setHeaderData(26, Qt::Horizontal, "Receipt Amount");
    roModel->setHeaderData(27, Qt::Horizontal, "MediaBill Amount");
    roModel->setHeaderData(28, Qt::Horizontal, "Rate CGST");
    roModel->setHeaderData(29, Qt::Horizontal, "Amount CGST");
    roModel->setHeaderData(30, Qt::Horizontal, "Rate SGST");
    roModel->setHeaderData(31, Qt::Horizontal, "Amount SGST");
    roModel->setHeaderData(32, Qt::Horizontal, "Rate IGST");
    roModel->setHeaderData(33, Qt::Horizontal, "Amount IGST");
    roModel->setHeaderData(34, Qt::Horizontal, "Final Amount");
    roModel->setHeaderData(35, Qt::Horizontal, "HSN CODE");

    paymentModel = new QSqlTableModel;
    paymentModel->setTable("payment");
    paymentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    paymentModel->select();
    paymentModel->setHeaderData(0, Qt::Horizontal, "ID");
    paymentModel->setHeaderData(1, Qt::Horizontal, "RO Code");
    paymentModel->setHeaderData(2, Qt::Horizontal, "RO Number");
    paymentModel->setHeaderData(3, Qt::Horizontal, "Date");
    paymentModel->setHeaderData(4, Qt::Horizontal, "Amount");
    paymentModel->setHeaderData(5, Qt::Horizontal, "Mode");
    paymentModel->setHeaderData(6, Qt::Horizontal, "Cheque No.");
    paymentModel->setHeaderData(7, Qt::Horizontal, "Bank Name");

    receiptModel = new QSqlTableModel;
    receiptModel->setTable("receipt");
    receiptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    receiptModel->select();
    receiptModel->setHeaderData(0, Qt::Horizontal, "ID");
    receiptModel->setHeaderData(1, Qt::Horizontal, "Number");
    receiptModel->setHeaderData(2, Qt::Horizontal, "RO Code");
    receiptModel->setHeaderData(3, Qt::Horizontal, "RO No.");
    receiptModel->setHeaderData(4, Qt::Horizontal, "Receipt Date");
    receiptModel->setHeaderData(5, Qt::Horizontal, "Receipt Amount");
    receiptModel->setHeaderData(6, Qt::Horizontal, "Pay Mode");
    receiptModel->setHeaderData(7, Qt::Horizontal, "Cheque");
    receiptModel->setHeaderData(8, Qt::Horizontal, "Bank Name");
    receiptModel->setHeaderData(9, Qt::Horizontal, "Remark");

    mediaBillModel = new QSqlTableModel;
    mediaBillModel->setTable("mediaBill");
    mediaBillModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mediaBillModel->select();
    mediaBillModel->setHeaderData(0, Qt::Horizontal, "ID");
    mediaBillModel->setHeaderData(1, Qt::Horizontal, "Date");
    mediaBillModel->setHeaderData(2, Qt::Horizontal, "RO Code");
    mediaBillModel->setHeaderData(3, Qt::Horizontal, "RO No.");
    mediaBillModel->setHeaderData(4, Qt::Horizontal, "Amount");

    generateBillModel = new QSqlQueryModel;
    generateBillModel->setQuery("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro");
    generateBillModel->setHeaderData(6, Qt::Horizontal, "RO No.");
    generateBillModel->setHeaderData(5, Qt::Horizontal, "Bill No.");
    generateBillModel->setHeaderData(2, Qt::Horizontal, "Media House");
    generateBillModel->setHeaderData(3, Qt::Horizontal, "Clients");
    generateBillModel->setHeaderData(16, Qt::Horizontal, "Net Amount");
    generateBillModel->setHeaderData(5, Qt::Horizontal, "HSN");    

}

SQLiteHandler::~SQLiteHandler()
{

}

