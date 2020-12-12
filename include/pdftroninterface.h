#ifndef PDFTRONINTERFACE_H
#define PDFTRONINTERFACE_H

#include <QObject>

#include <PDF/PDFNet.h>
#include <PDF/PDFDoc.h>
#include <PDF/Image.h>
#include <PDF/ContentReplacer.h>

#include <QStringList>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>

using namespace std;
using namespace pdftron;
using namespace Common;
using namespace SDF;
using namespace PDF;


class PDFTronInterface
{
    PDFTronInterface();
    QString pdfApplication;
    QProcess *process;
public:
    ~PDFTronInterface();

    static PDFTronInterface *get();
    static PDFTronInterface *instance;
    void showPdf(QString url);

    void printRO(QStringList detailList, QStringList mediaPaymentList, QString fileName="");
    void printReceipt(QStringList detailList,QStringList roDetail, QString fileName="");
    void printInvoice(QStringList dataList, QStringList roDetail, QString fileName="");
};

#endif // PDFTRONINTERFACE_H
