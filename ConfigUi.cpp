#include "ConfigUi.h"
#include "IOHandler/IOHandler.h"

ConfigUI::ConfigUI(IOHandler *_io, QWidget *parent): QDialog(parent), io(_io)
{
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
    pdfChoose = new QPushButton("Browse",this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(new QLabel("PDF Application"));
    hbox->addWidget(pdfApplicationUrl);
    hbox->addWidget(pdfChoose);
    QGroupBox *gb = new QGroupBox("PDF Application");
    gb->setLayout(hbox);

    vbox->addWidget(gb);
    vbox->addStretch();
    pdfWidget->setLayout(vbox);

    tabWidget->addTab(pdfWidget, "Print");
    mainLayout->addWidget(tabWidget);


    saveButton = new QPushButton("Save", this);
    closeButton = new QPushButton("Close", this);
    hbox = new QHBoxLayout(this);
    hbox->addStretch();
    hbox->addWidget(saveButton);
    hbox->addWidget(closeButton);

    mainLayout->addLayout(hbox);

    signalSetup();
}

void ConfigUI::signalSetup()
{
    connect(pdfChoose, &QPushButton::clicked, [this]{
        auto file = QFileDialog::getOpenFileName(this, "Choose PDF Application");
        pdfApplicationUrl->setText(file);
    });

    connect(saveButton, &QPushButton::clicked, this, &ConfigUI::saveValues);
    connect(closeButton, &QPushButton::clicked, [&]{
       close();
    });
}

void ConfigUI::setValues()
{
    auto strList = io->sql->getConfigList();
    pdfApplicationUrl->setText(strList[0]);
}

void ConfigUI::saveValues()
{
    io->sql->setConfig({pdfApplicationUrl->text()});
}
