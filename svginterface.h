#ifndef SVGINTERFACE_H
#define SVGINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>
#include <QDir>

#include "Log.h"

class Configure;

class SVGInterface
{
public:
    SVGInterface();

    QProcess *process;
    Configure *configure;
    void multiLine(int perLine, QString string, QString key, QByteArray &stream, int loop = 3);
    const std::string rightPadding(std::string string, int spaceSize);
    std::shared_ptr<spdlog::logger> log;
public:
    ~SVGInterface();

    static SVGInterface *get();
    static SVGInterface *instance;
    void showPdf(QString url);

    void printRO(QStringList detailList, QStringList mediaPaymentList);
    void printReceipt(QStringList detailList,QStringList roDetail);
    void printInvoice(QStringList dataList, QStringList roDetail);
};

#endif // SVGINTERFACE_H
