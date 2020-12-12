#include "AddInvoice.h"
#include "pdftroninterface.h"

AddInvoice::AddInvoice(QWidget *parent, int _invno, int _rono): QWidget(parent), io{IOHandler::getInstance()}, latestInvoiceCode(io->sql->getNewInvoiceCode())
{

    render();
    roNo->setText(QString::number(_rono));
    setLayout(mainLayout);
}

QComboBox *AddInvoice::getClients() const
{
    return clients;
}

void AddInvoice::render()
{

    roNo = new QLineEdit;
    roNo->setReadOnly(true);
    invoiceNo = new QComboBox;
    invoiceNoCurrent = new QLineEdit;
    invoiceNoCurrent->setReadOnly(true);
    invNoAdd = new QToolButton();
    invNoAdd->setText("+");
    invNoRemove = new QToolButton;
    invNoRemove->setText("-");
    date = new QDateEdit;
    date->setDisplayFormat("dd/MM/yyyy");
    clients = new QComboBox();
    clients->addItems(io->sql->getClientList());
    clients->setEditable(true);
    clients->setInsertPolicy(QComboBox::InsertPolicy::NoInsert);
//    clientEdit = new QLineEdit;
//    clientEdit->setReadOnly(true);

    grossAmt = new QLineEdit;
    discount = new QLineEdit;
    discount->setPlaceholderText(QString::number(0));
    discountPerc = new QDoubleSpinBox;
    discountPerc->setRange(0, 100);
//    discountPerc->setSuffix(" %");
    netPayableAmount = new QLineEdit;
    cgstPerc = new QComboBox;
    cgstPerc->addItems(*io->sql->getGstPerc());
    CGST = new QLineEdit;
    CGST->setReadOnly(true);
    sgstPerc = new QComboBox;
    sgstPerc->addItems(*io->sql->getGstPerc());
    SGST = new QLineEdit;
    SGST->setReadOnly(true);
    igstPerc = new QComboBox;
    igstPerc->addItems(*io->sql->getGstPerc());
    IGST = new QLineEdit;
    IGST->setReadOnly(true);
    invoiceAmount = new QLineEdit;
    invoiceAmount->setReadOnly(true);
    remark = new QTextEdit;
    totSizeDuration = new QLineEdit;
    save = new QPushButton("Save");
    remove = new QPushButton("Remove");
    clear = new QPushButton("Clear");
    print = new QPushButton("Print");
    print->setShortcut(QKeySequence("Ctrl+p"));

    mainLayout = new QVBoxLayout;
    QFormLayout *form = new QFormLayout;
    form->addRow("RO No.", roNo);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(invoiceNoCurrent, 1);
    hbox->addWidget(invoiceNo, 1);
    hbox->addWidget(invNoAdd, 2);
    hbox->addWidget(invNoRemove, 2);
    form->addRow("Invoice", hbox);
    form->addRow("Invoice Date", date);

    form->addRow("Parties", clients);
    form->addRow("Gross Amt", grossAmt);
    hbox = new QHBoxLayout;
    hbox->addWidget(discountPerc);
    hbox->addWidget(new QLabel("="));
    hbox->addWidget(discount);
    form->addRow("Discount @", hbox);

    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(netPayableAmount);
    form->addRow("Net Payable Amount",hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(cgstPerc);
    hbox->addWidget(new QLabel("="));
    hbox->addWidget(CGST);
    form->addRow("CGST @", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(sgstPerc);
    hbox->addWidget(new QLabel("="));
    hbox->addWidget(SGST);
    form->addRow("SGST @", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(igstPerc);
    hbox->addWidget(new QLabel("="));
    hbox->addWidget(IGST);
    form->addRow("IGST @",hbox);

    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(invoiceAmount);
    form->addRow("Invoice Amount", hbox);

    form->addRow("Remark", remark);
    form->addRow("Tot. Size/Duration", totSizeDuration);
    mainLayout->addLayout(form);

    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(remove);
    hbox->addWidget(clear);
    hbox->addWidget(print);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    setupSignal();
    populateData();
    setDefaultValue();
    setValidator();
}

void AddInvoice::setupSignal()
{

    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Warning, "Do you want to save/update", QString("Want to save/update Invoice numeber <b>%0<b>").arg(invoiceNo->currentText()), QMessageBox::Button::Yes | QMessageBox::Button::No);
        if(QMessageBox::Button::No == msg.exec())
            return;
        io->sql->insertInvoiceList(toStringList());
        emit saveClicked();
    });

    connect(print, &QPushButton::clicked, [this]{
        auto invoiceList = io->sql->getInvoiceList(invoiceNoCurrent->text().toInt());
        auto roData = io->sql->getROStringList(roNo->text().toInt());

        if(!invoiceList.isEmpty())
            PDFTronInterface::get()->printInvoice(invoiceList, roData);
    });

    connect(invoiceNo, &QComboBox::currentTextChanged, [=](const QString invno){
        if(invno.isEmpty())
            return;
        invoiceNoCurrent->setText(invno);
        auto valueList = io->sql->getInvoiceList(invno.toInt());
        this->setValue(valueList);
    });

//    connect(clients, &QComboBox::currentTextChanged, clientEdit, &QLineEdit::setText);

    connect(invNoAdd, &QToolButton::clicked, this, &AddInvoice::addNewInvoice);
    connect(invNoRemove, &QToolButton::clicked, this, &AddInvoice::removeInvoice);


    connect(discountPerc, &QDoubleSpinBox::textChanged, [=](const QString number){
        if(discount->hasFocus())
            return;
        auto discountAmount = grossAmt->text().toDouble() * ((number.toDouble()/100));
        discount->setText(QString::number(discountAmount));
        netPayableAmount->setText(QString::number(grossAmt->text().toDouble()-discountAmount));
    });

    connect(discount, &QLineEdit::textChanged, [=](const QString number){
        if(discountPerc->hasFocus())
            return;
        auto perc = (number.toDouble()/grossAmt->text().toDouble())*100;
        discountPerc->setValue(perc);
        netPayableAmount->setText(QString::number(grossAmt->text().toDouble()-number.toDouble()));
    });

    connect(cgstPerc, &QComboBox::currentTextChanged, [=](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
            tax = QString::number(0);
        else
            igstPerc->setCurrentIndex(0);
        auto cgstAmount = (tax.toDouble()/100)*netPayableAmount->text().toDouble();
        CGST->setText(QString::number(cgstAmount));
        invoiceAmount->setText(QString::number(netPayableAmount->text().toDouble() + cgstAmount + SGST->text().toDouble()));
    });

    connect(sgstPerc, &QComboBox::currentTextChanged, [=](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
            tax = QString::number(0);
        else
            igstPerc->setCurrentIndex(0);
//        igstPerc->setCurrentIndex(0);
        auto sgstAmount = (tax.toDouble()/100)*netPayableAmount->text().toDouble();
        SGST->setText(QString::number(sgstAmount));
        invoiceAmount->setText(QString::number(netPayableAmount->text().toDouble() + sgstAmount + CGST->text().toDouble()));
    });

    connect(igstPerc, &QComboBox::currentTextChanged, [=](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
        {
            IGST->setText(QString::number(0));
            return;
        }

        sgstPerc->setCurrentIndex(0);
        cgstPerc->setCurrentIndex(0);
        auto igstAmount = (tax.toDouble()/100) * netPayableAmount->text().toDouble();
        IGST->setText(QString::number(igstAmount));
        invoiceAmount->setText(QString::number(netPayableAmount->text().toDouble()+igstAmount));
    });

    connect(clients, &QComboBox::currentTextChanged, this, &AddInvoice::currentClientChanged);

}

void AddInvoice::setValue(const QStringList billList)
{

    if(billList.isEmpty())
    {
        clearValue();
        return;
    }
    roNo->setText(billList.at(0));

    date->setDate(QDate::fromString(billList.at(2), "dd/MM/yyyy"));
    clients->setCurrentText(billList.at(3));
    grossAmt->setText(billList.at(4));
    discountPerc->setValue(billList.at(5).toDouble());
//    discount->setText(billList.at(6));
//    netPayableAmount->setText(billList.at(7));
    cgstPerc->setCurrentText(billList.at(8));
//    CGST->setText(billList.at(9));
    sgstPerc->setCurrentText(billList.at(10));
//    SGST->setText(billList.at(11));
    igstPerc->setCurrentText(billList.at(12));
    IGST->setText(billList.at(13));
//    invoiceAmount->setText(billList.at(14));
    remark->setText(billList.at(15));
    totSizeDuration->setText(billList.at(16));

}

void AddInvoice::setValueFromRO(const QStringList roList)
{
//    clearValue();
    addNewInvoice();
    roNo->setText(roList.at(1));
    clients->setCurrentText(roList.at(6));
    totSizeDuration->setText(roList.at(13));
    grossAmt->setText(roList.at(19));
    discountPerc->setValue(0);


//    invoiceNo->setCurrentText(QString::number(latestInvoiceCode++));
//    roNo->setDisabled(true);
//    date->setDate(QDate::currentDate());
    //    clients->setDisabled(true);
}

void AddInvoice::setDefaultValue()
{
    discountPerc->setValue(0);
    discount->setText(QString::number(0));
    CGST->setText("0");
    SGST->setText("0");
    IGST->setText("0");
    cgstPerc->setCurrentText("");
    sgstPerc->setCurrentText("");
    igstPerc->setCurrentText("");


}

QStringList AddInvoice::toStringList()
{
    QStringList strList;
    strList << roNo->text()
            << invoiceNo->currentText()
            << date->text()
            << QString::number(io->sql->getClientCode(clients->currentText()))
            << grossAmt->text()
            << discountPerc->text()
            << discount->text()
            << netPayableAmount->text()
            << cgstPerc->currentText()
            << CGST->text()
            << sgstPerc->currentText()
            << SGST->text()
            << igstPerc->currentText()
            << IGST->text()
            << invoiceAmount->text()
            << remark->toPlainText()
            << totSizeDuration->text();

    return strList;
}

void AddInvoice::populateData()
{
    clients->addItems(io->dataEngine->clientStringList());
}

void AddInvoice::setValidator()
{
    grossAmt->setPlaceholderText("₹");
    grossAmt->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    netPayableAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    CGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    SGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    IGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    invoiceAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    discount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
//    discountPerc->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
}

void AddInvoice::clearValue()
{
//    roNo->setText("");
//    roNo->setDisabled(false);
//    invoiceNo->setCurrentText("");
//    invoiceNo->setEnabled(false);
    date->setDate(QDate::currentDate());
    clients->setCurrentText(QString::number(0));
    grossAmt->setText(QString::number(0));
    discount->setText(QString::number(0));
    netPayableAmount->setText(QString::number(0));
    cgstPerc->setCurrentText("");
    CGST->setText(QString::number(0));
    sgstPerc->setCurrentText("");
    SGST->setText(QString::number(0));
    igstPerc->setCurrentText("");
    IGST->setText(QString::number(0));
    invoiceAmount->setText(QString::number(0));
    remark->setText("");
    totSizeDuration->clear();
}

void AddInvoice::addNewInvoice()
{
//    auto rono = roNo->text();
//    auto client = clients->currentText();
    invoiceNo->insertItem(0, QString::number(latestInvoiceCode++));
    invoiceNo->setCurrentIndex(0);
//    roNo->setText(rono);
//    clients->setCurrentText(client);
}

void AddInvoice::removeInvoice()
{
    auto currentText = invoiceNo->currentText();
    if(currentText.isEmpty())
        return;
    invoiceNo->removeItem(invoiceNo->currentIndex());
}


AddInvoice::~AddInvoice()
{

}

void AddInvoice::setValueFromNumbers(const QStringList invnoList)
{
    invoiceNo->addItems(invnoList);
    invoiceNo->setCurrentIndex(0);

}
