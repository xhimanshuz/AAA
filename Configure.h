#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <memory>

#include "Log.h"
#include "IOHandler/SQLiteHandler.h"

class Configure
{
    std::shared_ptr<spdlog::logger> log;
    QString pdfApplication;
    QString databaseLocation;
    QString samepleFileLocation;
    QString roSaveLocation;
    QString invoiceSaveLocation;
    QString receiptSaveLocation;
    static Configure* instance;

    Configure() {log = spdlog::get("dlog");}
    Configure(const Configure&) {
        log = spdlog::get("dlog");
    }

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
        if(strList.isEmpty())
            return;
        setPdfApplication(strList[0]);
        setDatabaseLocation(strList[1]);
        setRoSaveLocation(strList[2]);
        setInvoiceSaveLocation(strList[3]);
        setReceiptSaveLocation(strList[4]);
        setSamepleFileLocation(strList[5]);

        log->info("Config Modified");
        printConfig();
    }

    void printConfig()
    {
        log->info("*********** CONFIG ***********"
                  "# PDF Application: {}\n"
                  "# Database location: {}\n"
                  "# RO Save Location: {}\n"
                  "# Invoice Save Location: {}\n"
                  "# Receipt Location: {}\n"
                  "# Sample Data Location: {}"
                  "", str(pdfApplication), str(databaseLocation), str(roSaveLocation), str(invoiceSaveLocation), str(receiptSaveLocation), str(samepleFileLocation));
    }
//    QString getDatabaseLocation() const;
//    void setDatabaseLocation(const QString &value);
//    QString getSamepleFileLocation() const;
//    void setSamepleFileLocation(const QString &value);
//    QString getRoSaveLocation() const;
//    void setRoSaveLocation(const QString &value);
//    QString getInvoiceSaveLocation() const;
//    void setInvoiceSaveLocation(const QString &value);
//    QString getReceiptSaveLocation() const;
//    void setReceiptSaveLocation(const QString &value);
    QString getSamepleFileLocation() const { return samepleFileLocation; }

    void setSamepleFileLocation(const QString &value) { samepleFileLocation = value; }

    QString getRoSaveLocation() const
    {
        return roSaveLocation;
    }

    void setRoSaveLocation(const QString &value)
    {
        roSaveLocation = value;
    }

    QString getInvoiceSaveLocation() const
    {
        return invoiceSaveLocation;
    }

    void setInvoiceSaveLocation(const QString &value)
    {
        invoiceSaveLocation = value;
    }

    QString getReceiptSaveLocation() const
    {
        return receiptSaveLocation;
    }

    void setReceiptSaveLocation(const QString &value)
    {
        receiptSaveLocation = value;
    }

    QString getDatabaseLocation() const
    {
        return databaseLocation;
    }

    void setDatabaseLocation(const QString &value)
    {
        databaseLocation = value;
    }
};


#endif
