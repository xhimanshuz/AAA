#include "IOHandler/SQLiteHandler.h"
#include <QDebug>
#include "Configure.h"

SQLiteHandler::SQLiteHandler(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    reloadDB();

}

void SQLiteHandler::reloadDB()
{
    db.setDatabaseName("AAA.db");
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
    query->exec("SELECT name FROM mediaHouse order by name");
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
        query->prepare("INSERT INTO clients (name, contactPerson, phone, email, address, city, state, GST, pincode) VALUES (:name, :contactPerson, :phone, :email, :address, :city, :state, :GST, :pincode)");
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
    if(auto result = query->exec())
        return result;
    else
    {
        auto error = query->lastError().text();
        qDebug()<< query->lastError();
    }
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
    query->exec("SELECT name FROM clients order by name");
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
        query->prepare("INSERT INTO ro (code,number,date,mhcode,mhname,pcode,pname,jobtypecode,jobtypename,caption,editCentre,doPubtel,sizeduration,totalsizeduration,guarantedpos,premium,strPre,rate,strRate,amount,netAmount,remarks,billAmount,invno,payamount,recptno,recptamount,mbamount,ratecgst,amountcgst,ratesgst,amountsgst,rateigst,amountigst,finalamount,hsncode,disPerc) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        query->bindValue(0, dataList.at(0));
        for(auto i=2; i< dataList.size(); i++)
        {
            query->bindValue(i, dataList.at(i));
        }
    }
    else
    {
        query->prepare("UPDATE ro SET code = :code, number = :number, date = :date, mhcode = :mhcode, mhname = :mhname, pcode = :pcode, pname = :pname, jobtypecode = :jobtypecode, jobtypename = :jobtypename, caption = :caption, editCentre = :editCentre, doPubtel = :doPubtel, sizeduration = :sizeduration, totalsizeduration = :totalsizeduration, guarantedpos = :guarantedpos, premium = :premium, strPre = :strPre, rate = :rate, strRate = :strRate, amount = :amount, netAmount = :netAmount, remarks = :remarks, billAmount = :billAmount, invno = :invno, payamount = :payamount, recptno = :recptno, recptamount = :recptamount, mbamount = :mbamount, ratecgst = :ratecgst, amountcgst = :amountcgst, ratesgst = :ratesgst, amountsgst = :amountsgst, rateigst = :rateigst, amountigst = :amountigst, finalamount = :finalamount, hsncode = :hsncode, disPerc = :disPerc WHERE number = ?");

        int i{0};
        for(; i< 37; i++)
        {
            query->addBindValue(dataList.at(i));
        }
        query->addBindValue(dataList.at(1));
//        auto s = query->lastQuery();

    }
    if(!query->exec())
    {
        auto q = query->lastQuery().toStdString();
        auto error = query->lastError().text();
        qDebug()<< error;
        return false;
    }
    return true;

}

const QStringList SQLiteHandler::getROStringList(const int id)
{
    query->exec(QString("SELECT * FROM ro WHERE number = %0 order by number").arg(id));
    if(!query->next())
    {
        qDebug()<< query->lastError();
        return {};
    }

    QStringList strList;
    for(auto i=0; i<37; i++)
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
    query->exec("SELECT number FROM ro order by number");
    QStringList strList;
    strList << "";
    while(query->next())
        strList << query->value(0).toString();

    return strList;
}

bool SQLiteHandler::updateNumberToRo(const QString &numbers, const int ro, SQLiteHandler::ROIndex index)
{
    QString columnName{""};
    switch (index)
    {
        case ROIndex::MH_CODE:
            columnName = "mhcode";
            break;
        case ROIndex::P_CODE:
            columnName = "pcode";
            break;
        case ROIndex::JOBT_CODE:
            columnName = "jobtypecode";
            break;
        case ROIndex::INV_NO:
            columnName = "invno";
            break;
        case ROIndex::RECPT_NO:
            columnName = "recptno";
            break;
        default:
        {
            qDebug()<< "Invalid Index";
            return false;
        }
    }

    query->prepare(QString("UPDATE ro SET %0=? WHERE number = ?").arg(columnName));
    query->bindValue(0, numbers);
    query->bindValue(1, ro);

    if(!query->exec())
    {
        auto q = query->lastError().text();
        qDebug()<< q;
        return false;
    }
    return true;

//    QString number{""};
//    if(query->next())
//        number = query->value(0).toString();





}

QSqlTableModel *SQLiteHandler::getMediaPaymentModel() const
{
    return paymentModel;
}

bool SQLiteHandler::insertMediaPayment(QList<QStringList> dataList, int rono)
{
    float total{0};
    query->prepare("DELETE FROM media_payment where rono = ?");
    query->bindValue(0, rono);
    if(!query->exec())
    {
        auto q = query->lastError();
        qDebug()<< "Error in removing";
        return false;
    }

    for(auto strList: dataList)
    {

            query->prepare("INSERT INTO media_payment (id, date, amount, mode, chequeNo, bankname, rono) VALUES (:id, :date, :amount, :mode, :chequeNo, :bankname, :rono);");
            query->bindValue(":id", strList.at(0).toInt());
            query->bindValue(":date", strList.at(1));
            query->bindValue(":amount", strList.at(2).toDouble());
            query->bindValue(":mode", strList.at(3));
            query->bindValue(":chequeNo", strList.at(4));
            query->bindValue(":bankname", strList.at(5));
            query->bindValue(":rono", rono);
        if(!query->exec())
        {
            auto k = query->lastError().text();
            qDebug()<<"Error in Inserting values in media_payment " << query->lastError().text();
            return false;
        }
        total+= strList[2].toDouble();
//        numberList.push_back(strList[0]);
    }
    
    query->prepare("UPDATE ro SET payamount = ? WHERE number = ?;");
    query->bindValue(0, total);
    query->bindValue(1, rono);
    if(!query->exec())
    {
        auto q = query->lastError().text();
        qDebug()<< q;
        return false;
    }
//    updateNumberToRo(numberList, ROIndex::RECPT_NO);
//    auto cn = combineNumber(numberList);

    return true;
}

QList<QStringList> SQLiteHandler::getMediaPaymentStringListByRono(int rono)
{
    query->exec(QString("SELECT id, date, amount, mode, chequeNo, bankname FROM media_payment WHERE rono = %0;").arg(rono));

    QList<QStringList> list;
    while(query->next())
    {
        QStringList strList;
        strList<< query->value(0).toString() << query->value(1).toString() << QString::number(query->value(2).toDouble()) << query->value(3).toString()<< query->value(4).toString()<< query->value(5).toString();
        list.append(strList);
    }

    return list;
}

int SQLiteHandler::getNewMediaPaymentNumber()
{
    query->exec("SELECT MAX(id) FROM media_payment;");

    if(!query->next())
        return -1;

    auto number = query->value(0).toInt();
    return number+1;
}

QStringList SQLiteHandler::getMediaPaymentStringList(int mpId)
{
    query->exec(QString("SELECT id, date, amount, mode, chequeNo, bankname FROM media_payment WHERE id = %0 order by id").arg(mpId));

    QStringList list;
    if(query->next())
    {
        list<< query->value(0).toString() << query->value(1).toString() << QString::number(query->value(2).toDouble()) << query->value(3).toString()<< query->value(4).toString()<< query->value(5).toString();
    }

    return list;
}

QSqlTableModel *SQLiteHandler::getPaymentReceiptModel() const
{
    return receiptModel;
}

bool SQLiteHandler::insertPaymentReceipt(QList<QStringList> dataList, int rono)
{
    QStringList idList;
    query->prepare("DELETE FROM payment_receipt where rono = ?");
    query->bindValue(0, rono);
    if(!query->exec())
    {
        auto q = query->lastError();
        qDebug()<< "Error in removing";
        return false;
    }

    double total{0.00};
    for(auto strList: dataList)
    {
        query->prepare("INSERT INTO payment_receipt (rcptdate, rcptamount, paymode, chqno, bankname, remark, rono, number) VALUES (:date, :amount, :paymode, :chqno, :bankname, :remark, :rono, :number);");

        query->bindValue(":date", strList.at(1));
        query->bindValue(":amount", strList.at(2).toDouble());
        query->bindValue(":paymode", strList.at(3));
        query->bindValue(":chqno", strList.at(4));
        query->bindValue(":bankname", strList.at(5));
        query->bindValue(":remark", strList.at(6));
        query->bindValue(":number", strList.at(0));
        query->bindValue(":rono", rono);
        if(!query->exec())
        {
            auto err = query->lastError().text();
            qDebug()<<"Error in Inserting values in receipt " << query->lastError();
            return false;
        }
        idList << strList[0];
        total+= strList[2].toDouble();
    }

    updateNumberToRo(combineNumber(idList), rono, ROIndex::RECPT_NO);
    query->prepare("UPDATE ro SET recptamount = ? WHERE number = ?;");
    query->bindValue(0, total);
    query->bindValue(1, rono);
    if(!query->exec())
    {
        auto q = query->lastError().text();
        qDebug()<< q;
        return false;
    }

    return true;

}

QList<QStringList> SQLiteHandler::getPaymentReceiptStringListByRO(int rono)
{
    query->exec(QString("SELECT number, rcptdate, rcptamount, paymode, chqno, bankname, remark  FROM payment_receipt WHERE rono = %0;").arg(rono));

    QList<QStringList> list;
    while(query->next())
    {
        QStringList strList;
        strList << query->value(0).toString()<< query->value(1).toString() << QString::number(query->value(2).toDouble()) << query->value(3).toString() << query->value(4).toString()<< query->value(5).toString()
                << query->value(6).toString();
        list.append(strList);
    }

    return list;
}

QStringList SQLiteHandler::getPaymentReceiptStringList(int prNo)
{
     query->exec(QString("SELECT number, rcptdate, rcptamount, paymode, chqno, bankname, remark  FROM payment_receipt WHERE number = %0 order by number;").arg(prNo));
     if(!query->next())
         return {};
     QStringList strList = QStringList() << query->value(0).toString()<< query->value(1).toString() << QString::number(query->value(2).toDouble()) << query->value(3).toString() << query->value(4).toString()<< query->value(5).toString()
                         << query->value(6).toString();
     return strList;
}

int SQLiteHandler::getNewPaymentReceiptNumber()
{
    query->exec("SELECT MAX(number) FROM payment_receipt;");
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
    query->exec(QString("SELECT id, date, amount FROM mediaBill WHERE rono = %0 order by id;").arg(rono));

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
    double total{0.0};
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
        total+= strList[2].toDouble();
    }
//    updateNumberToRo(combineNumber(), rono, ROIndex::RECPT_NO);
    query->prepare("UPDATE ro SET mbamount = ? WHERE number = ?;");
    query->bindValue(0, total);
    query->bindValue(1, rono);
    if(!query->exec())
    {
        auto q = query->lastError().text();
        qDebug()<< q;
        return false;
    }
    return true;
}


QSqlQueryModel *SQLiteHandler::getInvoiceModel() const
{
    return generateBillModel;
}

QStringList SQLiteHandler::getInvoiceList(const int invno)
{
    query->exec(QString("SELECT rono, number, date, pcode, gramount, disrate, disamount, npamount, ratecgst, amountcgst, ratesgst, amountsgst, rateigst, amountigst, finalamount, remark, totalsizeduration FROM invoice WHERE number = %0 order by number").arg(invno));
    if(!query->next())
    {
        qDebug()<<"Error for invoice id: "<< invno<< query->lastError();
        return {};
    }

//    auto gramount = query->value(4).toDouble();
    auto rono = query->value(0).toString();
    auto number = query->value(1).toString();
    auto date = query->value(2).toString();
    auto pcode = query->value(3).toString();
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
    auto remark = query->value(15).toString();
    auto totSizeDur = query->value(16).toString();

    QStringList strList;
    strList << rono
            << number
            << date
            << pcode
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
            << remark
            << totSizeDur;

    return strList;

}

bool SQLiteHandler::insertInvoiceList(QStringList strList)
{
    query->exec(QString("SELECT number FROM invoice WHERE number = %0").arg(strList.at(1).toInt()));
    if(!query->next())
    {
        qDebug() << "[!] "<<strList.at(1).toInt()<<" Invoice number doesn't exist, so, Inserting new one!" << query->lastError();
        query->prepare("INSERT INTO invoice ('rono','date', 'pcode', 'gramount', 'disrate','disamount', 'npamount', 'ratecgst','amountcgst','ratesgst','amountsgst','rateigst','amountigst','finalamount', 'remark', 'totalsizeduration') VALUES (:rono,:date,:pcode,:gramount,:disrate,:disamount,:npamount,:ratecgst,:amountcgst,:ratesgst,:amountsgst,:rateigst,:amountigst,:finalamount,:remark,:totalsizeduration)");
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
        query->bindValue(":remark", strList.at(15));
        query->bindValue(":totalsizeduration", strList.at(16));
    }
    else
    {
        qDebug()<<"[!] "<< strList.at(1).toInt()<<" Invoice number doesn't exist, so, Updating Existing one!";
        query->prepare("UPDATE invoice SET rono=:rono, date=:date, pcode=:pcode, gramount=:gramount, disrate=:disrate, disamount=:disamount, npamount=:npamount, ratecgst=:ratecgst, amountcgst=:amountcgst, ratesgst=:ratesgst, amountsgst=:amountsgst, rateigst=:rateigst, amountigst=:amountigst, finalamount=:finalamount, remark=:remark, totalsizeduration=:totalsizeduration WHERE number = :number");
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
        query->bindValue(":remark", strList.at(15));
        query->bindValue(":number", strList.at(1).toInt());
        query->bindValue(":totalsizeduration", strList.at(16));
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


    auto invno = strList.at(1);
    query->prepare("SELECT invno from ro WHERE number = ?");
    query->bindValue(0, strList.at(0).toInt());
    query->exec();
    if(query->next())
    {
        auto oldinvno = query->value(0).toString();
        if(!oldinvno.contains(invno))
        {
            oldinvno+= ","+invno;
        }
        invno = oldinvno;
    }

    query->prepare("UPDATE ro SET invno = :invno WHERE number = :number");
    query->bindValue(":invno", invno);
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

QList<QStringList> SQLiteHandler::getInvoiceListByRoNo(const int rono)
{
    try
    {

    query->exec(QString("SELECT number from invoice WHERE rono = %0").arg(rono));

    QList<QStringList> list;
    if(!query->next())
    while(query->next())
    {
        auto invNo = query->value(0).toInt();
        list << getInvoiceList(invNo);
    }

    return list;
    }
    catch(...)
    {
        qDebug()<< query->lastError();
        return {};
    }
}

QStringList *SQLiteHandler::getGstPerc() const
{
    return gstPerc;
}

bool SQLiteHandler::setConfig(const QStringList &configList)
{
    try
    {
        query->prepare("UPDATE config SET pdfapplication=?, database_location=?, rolocation=?, invoicelocation=?, receiptlocation=?, samplepdflocation=?;");
        for(auto& str: configList)
            query->addBindValue(str);

        if(!query->exec())
        {
            auto q= query->lastError().text();
            return false;
        }

    }
    catch(std::exception& e)
    {
        qDebug()<< "Exception occured"<<e.what();
        return false;
    }
    return true;
}

const QStringList SQLiteHandler::getConfigList() const
{
    QStringList strList;
    try {
        query->exec("Select * from config;");
        while(query->next())
            strList << query->value(0).toString() << query->value(1).toString() <<query->value(2).toString() <<query->value(3).toString() <<query->value(4).toString() <<query->value(5).toString();

    }
    catch (std::exception &e)
    {
        qDebug()<< "Exception Occured"<< e.what();
    }
    return strList;
}

const QString SQLiteHandler::combineNumber(const QStringList &numberList)
{
    QString combinedNumber{""};
    for(auto &number: numberList)
        combinedNumber += number+",";
    combinedNumber.resize(combinedNumber.size()-1);
    return combinedNumber;
}

void SQLiteHandler::setUpModels()
{
    gstPerc = new QStringList(QStringList()<< ""<< "2.5"<<"5.0"<<"6.0"<<"12.0"<<"9.0"<<"18.0"<<"14.0"<<"28.0");

    jobTypeModel = new QSqlQueryModel;
    jobTypeModel->setQuery("SELECT name from jobType ORDER BY name");
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

    roModel = new ROTableModel;
    roModel->setTable("ro");
    roModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    roModel->select();
    roModel->setFilter("mhcode != 0");
//    roModel->setHeaderData(0, Qt::Horizontal, "Code");
    roModel->setHeaderData(1, Qt::Horizontal, "Ro No.");
    roModel->setHeaderData(2, Qt::Horizontal, "Date");
//    roModel->setHeaderData(3, Qt::Horizontal, "M.H. Code");
//    roModel->setHeaderData(4, Qt::Horizontal, "M.H. Name");
//    roModel->setHeaderData(5, Qt::Horizontal, "Client Code");
    roModel->setHeaderData(6, Qt::Horizontal, "Client Name");
//    roModel->setHeaderData(7, Qt::Horizontal, "Job Type ID");
    roModel->setHeaderData(8, Qt::Horizontal, "Job Type");
//    roModel->setHeaderData(9, Qt::Horizontal, "Caption");
//    roModel->setHeaderData(10, Qt::Horizontal, "Edit Centre");
//    roModel->setHeaderData(11, Qt::Horizontal, "Do Pub Tel");
//    roModel->setHeaderData(12, Qt::Horizontal, "Size Duration");
//    roModel->setHeaderData(13, Qt::Horizontal, "Total Size Duration");
//    roModel->setHeaderData(14, Qt::Horizontal, "Guaranteed Position");
//    roModel->setHeaderData(15, Qt::Horizontal, "Premium");
//    roModel->setHeaderData(16, Qt::Horizontal, "Str. Premium");
//    roModel->setHeaderData(17, Qt::Horizontal, "Rate");
//    roModel->setHeaderData(18, Qt::Horizontal, "Str. Rate");
    roModel->setHeaderData(19, Qt::Horizontal, "Amount");
    roModel->setHeaderData(20, Qt::Horizontal, "Net Amount");
//    roModel->setHeaderData(21, Qt::Horizontal, "Remarks");
    roModel->setHeaderData(22, Qt::Horizontal, "Bill Amount");
    roModel->setHeaderData(23, Qt::Horizontal, "Invoice No.");
    roModel->setHeaderData(24, Qt::Horizontal, "Payment");
    roModel->setHeaderData(25, Qt::Horizontal, "Receipt No.");
    roModel->setHeaderData(26, Qt::Horizontal, "Receipt Amount");
    roModel->setHeaderData(27, Qt::Horizontal, "MediaBill Amount");
//    roModel->setHeaderData(28, Qt::Horizontal, "Rate CGST");
//    roModel->setHeaderData(29, Qt::Horizontal, "Amount CGST");
//    roModel->setHeaderData(30, Qt::Horizontal, "Rate SGST");
//    roModel->setHeaderData(31, Qt::Horizontal, "Amount SGST");
//    roModel->setHeaderData(32, Qt::Horizontal, "Rate IGST");
//    roModel->setHeaderData(33, Qt::Horizontal, "Amount IGST");
    roModel->setHeaderData(34, Qt::Horizontal, "RO Amount");
//    roModel->setHeaderData(35, Qt::Horizontal, "HSN CODE");
//    roModel->setHeaderData(36, Qt::Horizontal, "Discount %");

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

    if(query->exec("CREATE TABLE IF NOT EXISTS config (pdfapplication TEXT, database_location TEXT, rolocation TEXT, invoicelocation TEXT, receiptlocation TEXT, samplepdflocation TEXT);"))
    {
        if(query->exec("SELECT * FROM config;"))
            if(!query->next())
                query->exec("INSERT INTO config VALUES ('', 'AAA.db', 'Data/ro', 'Data/invoice', 'Data/receipt', 'Data/SamplePdf');");
    }
    else
    {
        auto q = query->lastError().text();
    }
    qDebug()<< "Model Setup";


}

SQLiteHandler::~SQLiteHandler()
{

}

