#include "pdftroninterface.h"
#include "PopplerInterface.h"
#include <QDebug>
#include "Configure.h"

PDFTronInterface* PDFTronInterface::instance = nullptr;

PDFTronInterface::PDFTronInterface()
{
    configure = Configure::get();
    PDFNet::Initialize();
    process = new QProcess();
}

PDFTronInterface::~PDFTronInterface()
{
    qDebug()<<" Closing PDFTron..!";
    PDFNet::Terminate();
}

PDFTronInterface *PDFTronInterface::get()
{
    if(!instance)
        instance = new PDFTronInterface;
    return instance;
}

void PDFTronInterface::showPdf(QString url)
{
    process->setProgram(configure->getPdfApplication());
    process->setArguments(QStringList()<< url);
    process->startDetached();
}

void PDFTronInterface::printRO(QStringList detailList, QStringList mediaPaymentList)
{
    std::string saveFile;
    try
    {
        PDFDoc doc(QString(configure->getSamepleFileLocation()+"/ro.pdf").toStdString());
        doc.InitSecurityHandler();

        // first, replace the image on the first page---
        ContentReplacer replacer;
        Page page = doc.GetPage(1);
        double amount = detailList[19].toDouble();
        double discountAmount =  (amount - detailList[20].toDouble());
        QString discountPerc = QString::number(((discountAmount / amount) * 100), 'g', 2);


        replacer.AddString("CLIENT", detailList.at(6).toUpper().toStdString());
        replacer.AddString("MEDIA_HOUSE", detailList.at(4).toUpper().toStdString());
        replacer.AddString("MEDIA_HOUSE_ADDRESS", detailList.at(42).toStdString());
        replacer.AddString("MEDIA_HOUSE_CITY", QString("%0, %1").arg(detailList.at(43)).arg(detailList[44]).toStdString());
        replacer.AddString("RONO", detailList.at(1).toStdString());
        multiLine(60, detailList[21].toUpper(), "REMARK", replacer, 3);
        replacer.AddString("RO_DATE", detailList.at(2).toStdString());
        replacer.AddString("CAPTION", detailList.at(9).toUpper().toStdString());
        replacer.AddString("EDITION_CENTRE", detailList.at(10).toUpper().toStdString());
        replacer.AddString("DOP", detailList.at(11).toStdString());
        replacer.AddString("SIZE_DURATION", detailList.at(12).toUpper().toStdString());
        replacer.AddString("TOTAL_SIZE_DURATION", detailList.at(13).toUpper().toStdString());
        replacer.AddString("GUARANTED_POSITION", detailList.at(14).toUpper().toStdString());
        replacer.AddString("PREMIUM", detailList.at(15).toStdString());
        replacer.AddString("RATE", detailList.at(17).toStdString());
        replacer.AddString("RATE_REMARK", detailList.at(18).toStdString());
        replacer.AddString("AMOUNT", detailList.at(19).toStdString());
        replacer.AddString("NET_AMOUNT", detailList.at(20).toStdString());
        replacer.AddString("CGST", detailList.at(28).toStdString());
        replacer.AddString("CGST_RATE", detailList.at(29).toStdString());
        replacer.AddString("SGST", detailList.at(30).toStdString());
        replacer.AddString("SGST_RATE", detailList.at(31).toStdString());
        replacer.AddString("IGST", detailList.at(32).toStdString());
        replacer.AddString("IGST_RATE", detailList.at(33).toStdString());
        replacer.AddString("RO_AMOUNT", detailList.at(34).toStdString());
        replacer.AddString("DATE", mediaPaymentList.at(1).toStdString());
        replacer.AddString("AMOUNT_2", mediaPaymentList.at(2).toStdString());
        replacer.AddString("MODE", mediaPaymentList.at(3).toStdString());
        replacer.AddString("CHEQUE_NO", mediaPaymentList.at(4).toStdString());
        replacer.AddString("BANK_NAME", mediaPaymentList.at(5).toStdString());
<<<<<<< HEAD
        replacer.AddString("DISCOUNT", (discountPerc+"%").toStdString());
=======
        replacer.AddString("DISCOUNT", discountPerc.toStdString());
>>>>>>> 8e76579adbd0367322ba4a7acfff78f765a9a009
        replacer.AddString("DISCOUNT_RATE", QString::number(discountAmount).toStdString());

        // finally, apply
        replacer.Process(page);
        saveFile = QString(configure->getRoSaveLocation()+QString("/RO_%0_%1.pdf").arg(detailList.at(1)).arg(mediaPaymentList.at(0))).toStdString();
        doc.Save(saveFile, SDFDoc::e_remove_unused, 0);
        qDebug() << "Done. Result saved in "<< saveFile.c_str();
        showPdf(saveFile.c_str());
    }
    catch (Common::Exception& e)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Error in PDFTron", QString(e.ToString().c_str()));
        msg.exec();
        //        qDebug() << e.ToString().c_str();
    }
    catch (...)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Error in PDFTron", "Unknow Error");
        msg.exec();
    }

    QMessageBox msg(QMessageBox::Icon::Information, "Sucess in PDFTron", QString("Succefully Created"));
    msg.exec();
    //    PopplerInterface pi;
    //    pi.toImage(saveFile.c_str());
}

void PDFTronInterface::printReceipt(QStringList detailList, QStringList roDetail)
{
    std::string saveFile;
    try
    {
        PDFDoc doc(QString(configure->getSamepleFileLocation()+"/recipt.pdf").toStdString());
        doc.InitSecurityHandler();

        // first, replace the image on the first page
        ContentReplacer replacer;
        Page page = doc.GetPage(1);
        // Image img = Image::Create(doc, input_path + "peppers.jpg");
        // replacer.AddImage(page.GetMediaBox(), img.GetSDFObj());
        // next, replace the text place holders on the second page
        replacer.AddString("NAME", roDetail.at(1).toStdString());
        replacer.AddString("AMOUNT", detailList.at(2).toStdString());
        replacer.AddString("MODE", detailList.at(3).toStdString());
        replacer.AddString("ADVERTISINGDATE", detailList.at(1).toStdString());
        replacer.AddString("MEDIA-HOUSE", roDetail.at(2).toStdString());
        replacer.AddString("RECEIPT", detailList.at(0).toStdString());
        replacer.AddString("RONO", roDetail.at(0).toStdString());
        replacer.AddString("DATE", roDetail.at(3).toStdString());
        replacer.AddString("GSTN", roDetail.at(4).toStdString());
        replacer.AddString("AMOUNT2", detailList.at(2).toStdString());

        // finally, apply
        replacer.Process(page);
        saveFile = QString(configure->getReceiptSaveLocation()+ QString("/receipt_%0.pdf").arg(detailList.at(0))).toStdString();
        doc.Save(saveFile, SDFDoc::e_remove_unused, 0);
        qDebug() << "Done. Result saved in "<< saveFile.c_str();
        showPdf(saveFile.c_str());
    }
    catch (Common::Exception& e)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Error in PDFTron", QString(e.ToString().c_str()));
        msg.exec();
        //        qDebug() << e.ToString().c_str();
    }
    catch (...)
    {
        QMessageBox msg(QMessageBox::Icon::Critical, "Error in PDFTron", "Unknow Error");
        msg.exec();
    }

    QMessageBox msg(QMessageBox::Icon::Information, "Sucess in PDFTron", QString("Succefully Created"));
    msg.exec();
    //    PopplerInterface pi;
    //    pi.toImage(saveFile.c_str());
}

void PDFTronInterface::multiLine(int perLine, QString string, QString key, ContentReplacer& replacer, int loop)
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
        replacer.AddString(QString("%0_%1").arg(key).arg(i++).toStdString(), str.toStdString());
}

const string PDFTronInterface::rightPadding(std::string string, int spaceSize)
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

void PDFTronInterface::printInvoice(QStringList dataList, QStringList roDetail)
{
    std::string saveFile;
    //    try {

    PDFDoc doc(QString(configure->getSamepleFileLocation()+"/invoice.pdf").toStdString());
    doc.InitSecurityHandler();

    ContentReplacer replacer;
    Page page = doc.GetPage(1);


    replacer.AddString(QString("RONO").toStdString(), dataList.at(0).toStdString());
    multiLine(23, roDetail.at(4),"MEDIA_CENTRE", replacer);
//    replacer.AddString(QString("MEDIA_CENTRE").toStdString(), roDetail.at(4).toStdString());
    replacer.AddString(QString("DATE").toStdString(), dataList.at(2).toStdString());
    multiLine(15, roDetail.at(12), "SIZE_DUR", replacer);
    multiLine(15, dataList.at(16), "TOTAL_DUR", replacer);
    //            replacer.AddString(QString("TOTAL_DUR").toStdString(), dataList.at(16).toStdString());
    replacer.AddString(QString("HSN").toStdString(), roDetail.at(35).toStdString());
    replacer.AddString(QString("RATE").toStdString(), rightPadding(roDetail.at(17).toStdString(), 8));
    replacer.AddString(QString("PREMIUM").toStdString(),rightPadding(dataList.at(0).toStdString(), 13));
    replacer.AddString(QString("AMOUNT").toStdString(), rightPadding(dataList.at(4).toStdString(), 13));

    replacer.AddString(QString("GROSS_AMOUNT").toStdString(), rightPadding(dataList.at(4).toStdString(), 20));
    replacer.AddString(QString("DISCOUNT").toStdString(), rightPadding(dataList.at(6).toStdString(), 20));
    replacer.AddString(QString("AMT_AFT_DIS").toStdString(), rightPadding(dataList.at(7).toStdString(), 20));
    replacer.AddString(QString("CGST").toStdString(), rightPadding(dataList.at(9).toStdString(), 20));
    replacer.AddString(QString("SGST").toStdString(), rightPadding(dataList.at(11).toStdString(), 20));
    replacer.AddString(QString("IGST").toStdString(), rightPadding(dataList.at(12).toStdString(), 20));
    replacer.AddString(QString("INVOICE_AMT").toStdString(), rightPadding(dataList.at(14).toStdString(), 20));
    replacer.AddString(QString("CLIENT").toStdString(), dataList.at(17).toStdString());
    multiLine(60, dataList.at(18), "CLIENT_ADDRESS", replacer);
//    replacer.AddString("CLIENT_ADDRESS", dataList.at(18).toStdString());
    replacer.AddString(QString("CLIENT_CITY").toStdString(), (dataList.at(19)+", "+dataList[20]).toStdString());
    replacer.AddString(QString("INVOICE_DATE").toStdString(), dataList.at(2).toStdString());
    replacer.AddString(QString("INVOICE_NO").toStdString(), dataList.at(1).toStdString());
    replacer.AddString(QString("CAPTION").toStdString(), roDetail.at(9).toStdString());
    replacer.AddString(QString("STATE").toStdString(), dataList.at(0).toStdString());
    replacer.AddString(QString("CODE").toStdString(), dataList[22].toStdString());
    multiLine(40, dataList.at(15), "REMARK", replacer, 4);
    //            replacer.AddString(QString("REMARK").toStdString(), dataList.at(15).toStdString());
    replacer.AddString(QString("STATE").toStdString(), dataList[20].toStdString());
    replacer.AddString(QString("GSTIN").toStdString(), dataList[21].toStdString());


    replacer.Process(page);
    saveFile = QString(configure->getInvoiceSaveLocation() +QString("/invoice_%0.pdf").arg(dataList.at(0))).toStdString();
    doc.Save(saveFile, SDFDoc::e_remove_unused, 0);
    qDebug() << "Done. Result saved in "<< saveFile.c_str();
    showPdf(saveFile.c_str());
}