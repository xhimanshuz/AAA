#ifndef CONFIGUI_H
#define CONFIGUI_H

class IOHandler;

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include "Log.h"

class ConfigUI : public QDialog
{
    enum LOG_LEVEL
    {
        NONE = 0,
        INFO = 1,
        DEBUG = 2
    };

    Q_OBJECT

    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;
    QLineEdit *pdfApplicationUrl;
    QLineEdit *dataBaseUrl;
    QLineEdit *roPdfUrl;
    QLineEdit *invoicePdfUrl;
    QLineEdit *receiptPdfUrl;
    QLineEdit *samplePdfUrl;

    QPushButton *pdfAppChoose;
    QPushButton *roPdfChoose;
    QPushButton *dbChoose;
    QPushButton *invoiceChoose;
    QPushButton *receiptChoose;
    QPushButton *samplePdfChoose;
    QPushButton *saveButton;
    QPushButton *closeButton;
    QPushButton *updateDataBaseButton;
    QRadioButton *infoLogs;
    QRadioButton *debugLogs;
    QRadioButton *noLogs;
    QButtonGroup *buttonGroup;
    LOG_LEVEL current;

    IOHandler *io;

    std::shared_ptr<spdlog::logger> log;
    void render();
    void signalSetup();
    void setValues();
    void saveValues();
public:
    ConfigUI(IOHandler *_io, QWidget *parent = nullptr);

};

#endif // CONFIGUI_H
