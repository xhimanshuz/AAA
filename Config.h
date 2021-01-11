#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <memory>
#include "IOHandler/SQLiteHandler.h"


class Configure
{
    QString pdfApplication;
    static Configure* instance;

    Configure() {}
    Configure(const Configure&) {}

public:
    virtual ~Configure(){}
    static Configure* get()
    {
        if(!instance)
            instance = new Configure;
        return instance;
    }

    void setPdfApplication(const QString &_pdfApplication)
    {
        pdfApplication = _pdfApplication;
    }

    const QString getPdfApplication()
    {
        return pdfApplication;
    }

    void setConfigFromList(const QStringList &strList)
    {
        setPdfApplication(strList[0]);
    }

};

#pragma once
Configure* Configure::instance = nullptr;


#endif // CONFIG_H
