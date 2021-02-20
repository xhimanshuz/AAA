#include "ConfigUi.h"
#include "IOHandler/IOHandler.h"
#include "Configure.h"

ConfigUI::ConfigUI(IOHandler *_io, QWidget *parent): QDialog(parent), io(_io)
{
    log = spdlog::get("dlog");

    mainLayout = new QVBoxLayout(this);
    render();
    setLayout(mainLayout);
    setValues();
}

void ConfigUI::render()
{
    tabWidget = new QTabWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    QWidget *pdfWidget = new QWidget;

    pdfApplicationUrl = new QLineEdit(this);
    pdfAppChoose = new QPushButton("Browse",this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(new QLabel("PDF Application"));
    hbox->addWidget(pdfApplicationUrl);
    hbox->addWidget(pdfAppChoose);
//    gvbox->addLayout(hbox);
    QGroupBox *gb = new QGroupBox("PDF Application");
    gb->setLayout(hbox);
    vbox->addWidget(gb);

    QVBoxLayout *gvbox = new QVBoxLayout;
    hbox = new QHBoxLayout;
    roPdfUrl = new QLineEdit(this);
    roPdfChoose = new QPushButton("Browse", this);
    hbox->addWidget(new QLabel("RO PDF"));
    hbox->addWidget(roPdfUrl);
    hbox->addWidget(roPdfChoose);
    gvbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    invoicePdfUrl = new QLineEdit(this);
    invoiceChoose = new QPushButton("Browse", this);
    hbox->addWidget(new QLabel("Invoice PDF"));
    hbox->addWidget(invoicePdfUrl);
    hbox->addWidget(invoiceChoose);
    gvbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    receiptPdfUrl = new QLineEdit(this);
    receiptChoose = new QPushButton("Browse", this);
    hbox->addWidget(new QLabel("Receipt PDF"));
    hbox->addWidget(receiptPdfUrl);
    hbox->addWidget(receiptChoose);
    gvbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    samplePdfUrl = new QLineEdit(this);
    samplePdfChoose = new QPushButton("Browse", this);
    hbox->addWidget(new QLabel("Sample PDF"));
    hbox->addWidget(samplePdfUrl);
    hbox->addWidget(samplePdfChoose);
    gvbox->addLayout(hbox);


    gb = new QGroupBox("PDF Directory Save");
    gb->setLayout(gvbox);
    vbox->addWidget(gb);


    vbox->addStretch();
    pdfWidget->setLayout(vbox);

    tabWidget->addTab(pdfWidget, "Print");

    QWidget *dbWidget = new QWidget;
    gvbox = new QVBoxLayout;
    gb = new QGroupBox("Database File");
    dataBaseUrl = new QLineEdit(this);
    dataBaseUrl->setDisabled(true);
    dbChoose = new QPushButton("Browse", this);
    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Database File"));
    hbox->addWidget(dataBaseUrl);
    hbox->addWidget(dbChoose);
    gb->setLayout(hbox);
    gvbox->addWidget(gb);
    gvbox->addStretch();
    dbWidget->setLayout(gvbox);
    tabWidget->addTab(dbWidget, "Database");

    QWidget *logsWidget = new QWidget;
    infoLogs = new QRadioButton("Info");
    debugLogs = new QRadioButton("Debug");
    noLogs = new QRadioButton("No Logs");
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(noLogs, LOG_LEVEL::NONE);
    buttonGroup->addButton(infoLogs, LOG_LEVEL::INFO);
    buttonGroup->addButton(debugLogs, LOG_LEVEL::DEBUG);
    hbox = new QHBoxLayout;
    hbox->addWidget(infoLogs);
    hbox->addWidget(debugLogs);
    hbox->addWidget(noLogs);
    gvbox = new QVBoxLayout;
    gvbox->addLayout(hbox);
    gb = new QGroupBox("Logs Level");
    gb->setLayout(gvbox);
    vbox = new QVBoxLayout;
    vbox->addWidget(gb);
    vbox->addStretch();
    logsWidget->setLayout(vbox);
    tabWidget->addTab(logsWidget, "Logs");
    mainLayout->addWidget(tabWidget);

    saveButton = new QPushButton("Save", this);
    closeButton = new QPushButton("Close", this);
    hbox = new QHBoxLayout(this);
    hbox->addStretch();
    hbox->addWidget(saveButton);
    hbox->addWidget(closeButton);

    mainLayout->addLayout(hbox);

    signalSetup();
    debugLogs->setChecked(true);
}

void ConfigUI::signalSetup()
{
    connect(pdfAppChoose, &QPushButton::clicked, [this]{
        auto file = QFileDialog::getOpenFileName(this, "Choose PDF Application");
        pdfApplicationUrl->setText(file);
    });

    connect(dbChoose, &QPushButton::clicked, [this]{
        auto file = QFileDialog::getOpenFileName(this, "Choose Database");
        dataBaseUrl->setText(file);
    });

    connect(roPdfChoose, &QPushButton::clicked, [this]{
        auto dir = QFileDialog::getExistingDirectory(this, "Choose RO Location");
        roPdfUrl->setText(dir);
    });

    connect(invoiceChoose, &QPushButton::clicked, [this]{
        auto dir  = QFileDialog::getExistingDirectory(this, "Choose Invoice Location");
        invoicePdfUrl->setText(dir);
    });

    connect(receiptChoose, &QPushButton::clicked, [this]{
        auto dir = QFileDialog::getExistingDirectory(this, "Choose Receipt Location");
        receiptPdfUrl->setText(dir);
    });

    connect(samplePdfChoose, &QPushButton::clicked, [this]{
        auto dir = QFileDialog::getExistingDirectory(this, "Choose Sample Pdf Location");
        samplePdfUrl->setText(dir);
    });



    connect(saveButton, &QPushButton::clicked, this, &ConfigUI::saveValues);
    connect(closeButton, &QPushButton::clicked, [&]{
       close();
    });

    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id){
        switch (id)
        {
            case NONE:
                log->set_level(spdlog::level::off);
                current = NONE;
                break;
            case INFO:
                log->set_level(spdlog::level::info);
                current = INFO;
                break;
            case DEBUG:
                log->set_level(spdlog::level::debug);
                current = DEBUG;
                break;
            default:
                break;
        }

    } );

}

void ConfigUI::setValues()
{
    auto strList = io->sql->getConfigList();
    if(strList.empty())
        return;
    pdfApplicationUrl->setText(strList[0]);
    dataBaseUrl->setText("AAA.db");
    roPdfUrl->setText(strList[2]);
    invoicePdfUrl->setText(strList[3]);
    receiptPdfUrl->setText(strList[4]);
    samplePdfUrl->setText(strList[5]);
}

void ConfigUI::saveValues()
{
    if(io->sql->setConfig({pdfApplicationUrl->text(), dataBaseUrl->text(), roPdfUrl->text(), invoicePdfUrl->text(), receiptPdfUrl->text(), samplePdfUrl->text()}))
        Configure::get()->setConfigFromList(io->sql->getConfigList());
//    Configure::get()->setConfigFromList(io->sql->getConfigList());
}
