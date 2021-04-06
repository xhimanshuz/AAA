#include "svginterface.h"
#include <Configure.h>

SVGInterface* SVGInterface::instance = nullptr;

SVGInterface::SVGInterface()
{
    configure = Configure::get();
    process = new QProcess;
    log = spdlog::get("dlog");
}

void SVGInterface::multiLine(int perLine, QString string, QString key, QByteArray &stream, int loop)
{
    QStringList strList;
    std::string str{""};
    for(auto i=0; i<loop; i++)
    {
        if(perLine*i>string.size())
            str = "";
        else
        {
            str = string.toStdString().substr(i*perLine, perLine);
            if(i > 0 && !str.empty())
                strList[i-1]+="-";
        }
        strList << str.data();
    }

    int i =0;
    for(auto str: strList)
        stream.replace(QString("[%0_%1]").arg(key).arg(i++).toStdString().c_str(), str.toStdString().c_str());
}

const std::string SVGInterface::rightPadding(std::string string, int spaceSize)
{
    if(string.size() < 4)
        spaceSize+=3;
    if(string.find(".") == std::string::npos)
        string+=".00";
    else
    {
        auto i = string.find(".");
        auto size = string.size();
        if(i == string.size()-2)
            string+="0";
    }
    string.insert(0, spaceSize-string.size(), ' ');
    return string;
}

SVGInterface *SVGInterface::get()
{
    if(!instance)
        instance = new SVGInterface;
    return instance;
}

void SVGInterface::showPdf(QString url)
{
        process->setProgram(configure->getPdfApplication());
        process->setArguments(QStringList()<< url);
        process->startDetached();
}

void SVGInterface::printRO(QStringList detailList, QStringList mediaPaymentList)
{

    QString saveFile;
    try
    {
        QFile svgFile(QString(configure->getSamepleFileLocation()+"/ro.svg"));
        if(!svgFile.open(QIODevice::ReadOnly))
        {
            log->critical("Error in Reading SVG File: {} {}", str(svgFile.fileName()), svgFile.exists());
            svgFile.close();
        }
        assert(svgFile.open(QIODevice::ReadOnly));
        log->debug("File Opened! {}", str(svgFile.fileName()));

        double amount = detailList[19].toDouble();
        double discountAmount =  (amount - detailList[20].toDouble());
        QString discountPerc = QString::number(((discountAmount / amount) * 100), 'g', 2);
        auto stream = svgFile.readAll();

        stream.replace("[CLIENT]", detailList.at(6).toUpper().toStdString().c_str());
        stream.replace("[MEDIA_HOUSE]", detailList.at(4).toUpper().toStdString().c_str());
        stream.replace("[MEDIA_HOUSE_ADDRESS]", detailList.at(42).toStdString().c_str());
        stream.replace("[MEDIA_HOUSE_CITY]", QString("%0, %1").arg(detailList.at(43)).arg(detailList[44]).toStdString().c_str());
        stream.replace("[RONO]", detailList.at(1).toStdString().c_str());
        multiLine(60, detailList[21].toUpper(), "REMARK", stream, 3);
        stream.replace("[RO_DATE]", detailList.at(2).toStdString().c_str());
        stream.replace("[CAPTION]", detailList.at(9).toUpper().toStdString().c_str());
        stream.replace("[EDITION_CENTRE]", detailList.at(10).toUpper().toStdString().c_str());
        stream.replace("[DOP]", detailList.at(11).toStdString().c_str());
        stream.replace("[SIZE_DURATION]", detailList.at(12).toUpper().toStdString().c_str());
        stream.replace("[TOTAL_SIZE_DURATION]", detailList.at(13).toUpper().toStdString().c_str());
        stream.replace("[GUARANTED_POSITION]", detailList.at(14).toUpper().toStdString().c_str());
        stream.replace("[PREMIUM]", detailList.at(15).toStdString().c_str());
        stream.replace("[RATE]", detailList.at(17).toStdString().c_str());
        stream.replace("[RATE_REMARK]", detailList.at(18).toStdString().c_str());
        stream.replace("[AMOUNT]", detailList.at(19).toStdString().c_str());
        stream.replace("[NET_AMOUNT]", detailList.at(20).toStdString().c_str());
        stream.replace("[CGST]", detailList.at(28).toStdString().c_str());
        stream.replace("[CGST_RATE]", detailList.at(29).toStdString().c_str());
        stream.replace("[SGST]", detailList.at(30).toStdString().c_str());
        stream.replace("[SGST_RATE]", detailList.at(31).toStdString().c_str());
        stream.replace("[IGST]", detailList.at(32).toStdString().c_str());
        stream.replace("[IGST_RATE]", detailList.at(33).toStdString().c_str());
        stream.replace("[RO_AMOUNT]", detailList.at(34).toStdString().c_str());
        stream.replace("[DATE]", mediaPaymentList.at(1).toStdString().c_str());
        stream.replace("[AMOUNT_2]", mediaPaymentList.at(2).toStdString().c_str());
        stream.replace("[MODE]", mediaPaymentList.at(3).toStdString().c_str());
        stream.replace("[CHEQUE_NO]", mediaPaymentList.at(4).toStdString().c_str());
        stream.replace("[BANK_NAME]", mediaPaymentList.at(5).toStdString().c_str());
        stream.replace("[DISCOUNT]", (discountPerc+"%").toStdString().c_str());
        stream.replace("[DISCOUNT_RATE]", QString::number(discountAmount).toStdString().c_str());

        saveFile = QString(configure->getRoSaveLocation()+QString("/RO_%0_%1.svg").arg(detailList.at(1)).arg(mediaPaymentList.at(0)));

        QFile svgFileW(saveFile);
        if(!svgFileW.open(QIODevice::WriteOnly))
        {
            log->critical("Error in Writing SVG File: {} {}", str(svgFileW.fileName()), svgFileW.exists());
            svgFile.close();
            QMessageBox mb;
            mb.setText("Errorin Writing");
            mb.exec();
        }
//        assert(svgFileW.open(QIODevice::WriteOnly));
        svgFileW.write(stream);
        svgFile.close();
        svgFileW.close();

        showPdf(saveFile);
    }
    catch(std::exception &e)
    {
        throw std::runtime_error(e.what());
    }

}

void SVGInterface::printReceipt(QStringList detailList, QStringList roDetail)
{
    QString saveFile;
    try
    {
        QFile svgFile(QString(configure->getSamepleFileLocation()+"/receipt.svg"));

        if(!svgFile.open(QIODevice::ReadOnly))
        {
            log->critical("Error in Reading SVG File: {} {}", str(svgFile.fileName()), svgFile.exists());
            svgFile.close();
        }
        assert(svgFile.open(QIODevice::ReadOnly));
        log->debug("File Opened! {}", str(svgFile.fileName()));
        auto stream = svgFile.readAll();

        stream.replace("[NAME]", roDetail.at(1).toStdString().c_str());
        stream.replace("[AMOUNT]", detailList.at(2).toStdString().c_str());
        stream.replace("[MODE]", detailList.at(3).toStdString().c_str());
        stream.replace("[ADVERTISINGDATE]", detailList.at(1).toStdString().c_str());
        stream.replace("[MEDIA-HOUSE]", roDetail.at(2).toStdString().c_str());
        stream.replace("[RECEIPT]", detailList.at(0).toStdString().c_str());
        stream.replace("[RONO]", roDetail.at(0).toStdString().c_str());
        stream.replace("[DATE]", roDetail.at(3).toStdString().c_str());
        stream.replace("[GSTN]", roDetail.at(4).toStdString().c_str());
        stream.replace("[AMOUNT2]", detailList.at(2).toStdString().c_str());

        saveFile = QString(configure->getReceiptSaveLocation()+ QString("/receipt_%0.svg").arg(detailList.at(0)));

        QFile svgFileW(saveFile);
        if(!svgFileW.open(QIODevice::WriteOnly))
        {
            log->critical("Error in Writing SVG File: {} {}", str(svgFileW.fileName()), svgFileW.exists());
            svgFile.close();
            QMessageBox mb;
            mb.setText("Errorin Writing");
            mb.exec();
        }
        assert(svgFileW.open(QIODevice::WriteOnly));
        svgFileW.write(stream);
        svgFile.close();
        svgFileW.close();

        showPdf(saveFile);
    }
    catch(std::exception &e)
    {
        throw std::runtime_error(e.what());
    }
}

void SVGInterface::printInvoice(QStringList dataList, QStringList roDetail)
{
    QString saveFile;
    try
    {
        QFile svgFile(QString(configure->getSamepleFileLocation()+"/invoice.svg"));
        if(!svgFile.open(QIODevice::ReadOnly))
        {
            log->critical("Error in Reading SVG File: {} {}", str(svgFile.fileName()), svgFile.exists());
            svgFile.close();
        }
        assert(svgFile.open(QIODevice::ReadOnly));
        log->debug("File Opened! {}", str(svgFile.fileName()));
        auto stream = svgFile.readAll();

        stream.replace("[RONO]", dataList.at(0).toStdString().c_str());
        multiLine(23, roDetail.at(4),"MEDIA_CENTRE", stream);
        stream.replace("[DATE]", dataList.at(2).toStdString().c_str());
        multiLine(15, roDetail.at(12), "SIZE_DUR", stream);
        multiLine(15, dataList.at(16), "TOTAL_DUR", stream);
        stream.replace("[HSN]", roDetail.at(35).toStdString().c_str());
        stream.replace("[RATE]", rightPadding(roDetail.at(17).toStdString(), 8).c_str());
        stream.replace("[PREMIUM]",rightPadding(dataList.at(0).toStdString(), 13).c_str());
        stream.replace("[AMOUNT]", rightPadding(dataList.at(4).toStdString(), 13).c_str());
        stream.replace("[GROSS_AMOUNT]", rightPadding(dataList.at(4).toStdString(), 20).c_str());
        stream.replace("[DISCOUNT]", rightPadding(dataList.at(6).toStdString(), 20).c_str());
        stream.replace("[AMT_AFT_DIS]", rightPadding(dataList.at(7).toStdString(), 20).c_str());
        stream.replace("[CGST]", rightPadding(dataList.at(9).toStdString(), 20).c_str());
        stream.replace("[SGST]", rightPadding(dataList.at(11).toStdString(), 20).c_str());
        stream.replace("[IGST]", rightPadding(dataList.at(12).toStdString(), 20).c_str());
        stream.replace("[INVOICE_AMT]", rightPadding(dataList.at(14).toStdString(), 20).c_str());
        stream.replace("[CLIENT", dataList.at(17).toStdString().c_str());
        multiLine(60, dataList.at(18), "CLIENT_ADDRESS", stream);
        stream.replace("[CLIENT_CITY]", (dataList.at(19)+", "+dataList[20]).toStdString().c_str());
        stream.replace("[INVOICE_DATE]", dataList.at(2).toStdString().c_str());
        stream.replace("[INVOICE_NO]", dataList.at(1).toStdString().c_str());
        stream.replace("[CAPTION]", roDetail.at(9).toStdString().c_str());
        stream.replace("[STATE]", dataList.at(0).toStdString().c_str());
        stream.replace("[CODE]", dataList[22].toStdString().c_str());
        multiLine(40, dataList.at(15), "REMARK", stream, 4);
        stream.replace("[STATE]", dataList[20].toStdString().c_str());
        stream.replace("[GSTIN]", dataList[21].toStdString().c_str());

        saveFile = QString(configure->getInvoiceSaveLocation() +QString("/invoice_%0.svg").arg(dataList.at(0)));
        QFile svgFileW(saveFile);
        if(!svgFileW.open(QIODevice::WriteOnly))
        {
            log->critical("Error in Writing SVG File: {} {}", str(svgFileW.fileName()), svgFileW.exists());
            svgFile.close();
            QMessageBox mb;
            mb.setText("Errorin Writing");
            mb.exec();
        }
        assert(svgFileW.open(QIODevice::WriteOnly));
        svgFileW.write(stream);
        svgFile.close();
        svgFileW.close();

        showPdf(saveFile);
    }
    catch(std::exception &e)
    {
        throw std::runtime_error(e.what());
    }
}
