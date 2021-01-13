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

class Configure;

class PDFTronInterface
{
    PDFTronInterface();
//    QString pdfApplication;
    QProcess *process;
    Configure *configure;
    void multiLine(int perLine, QString string, QString key, ContentReplacer& replacer, int loop = 3);
public:
    ~PDFTronInterface();

    static PDFTronInterface *get();
    static PDFTronInterface *instance;
    void showPdf(QString url);

    void printRO(QStringList detailList, QStringList mediaPaymentList);
    void printReceipt(QStringList detailList,QStringList roDetail);
    void printInvoice(QStringList dataList, QStringList roDetail);
};

#endif // PDFTRONINTERFACE_H
