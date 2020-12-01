#include "AddInvoice.h"
#include "pdftroninterface.h"

AddInvoice::AddInvoice(QWidget *parent, const int _invno, const int rono): QWidget(parent)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

//    if(_invno != 0)
//        setValue(io->sql->getInvoiceList(_invno));
//    else if(rono > 0)
//        setValueFromRO(io->sql->getROStringList(rono));
}

QComboBox *AddInvoice::getClients() const
{
    return clients;
}

void AddInvoice::render()
{

    roNo = new QLineEdit;
    invoiceNo = new QComboBox;
    date = new QDateEdit;
    date->setDisplayFormat("dd/MM/yyyy");
    clients = new QComboBox();
    clients->addItems(io->sql->getClientList());

    grossAmt = new QLineEdit;
    discount = new QLineEdit;
    discountPerc = new QLineEdit;
    netPayableAmount = new QLineEdit;
    cgstPerc = new QComboBox;
    cgstPerc->addItems(*io->sql->getGstPerc());
    CGST = new QLineEdit;
    sgstPerc = new QComboBox;
    sgstPerc->addItems(*io->sql->getGstPerc());
    SGST = new QLineEdit;
    igstPerc = new QComboBox;
    igstPerc->addItems(*io->sql->getGstPerc());
    IGST = new QLineEdit;
    invoiceAmount = new QLineEdit;
    remark = new QTextEdit;
    totSizeDuration = new QListView;
    save = new QPushButton("Save");
    remove = new QPushButton("Remove");
    clear = new QPushButton("Clear");
    print = new QPushButton("Print");
    print->setShortcut(QKeySequence("Ctrl+p"));

    mainLayout = new QVBoxLayout;
    QFormLayout *form = new QFormLayout;
    form->addRow("RO No.", roNo);
    form->addRow("Invoice", invoiceNo);
    form->addRow("Invoice Date", date);
    form->addRow("Parties", clients);
    form->addRow("Gross Amt", grossAmt);
    QHBoxLayout *hbox = new QHBoxLayout;
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

    populateData();
    setupSignal();
    setValidator();
}

void AddInvoice::setupSignal()
{
    connect(save, &QPushButton::clicked, [this]{
        io->sql->insertInvoiceList(toStringList());
        emit saveClicked();
    });

    connect(print, &QPushButton::clicked, [this]{
        auto invoiceList = io->sql->getInvoiceListByRoNo(roNo->text().toInt());
        auto roData = io->sql->getROStringList(roNo->text().toInt());

        PDFTronInterface::get()->printInvoice(invoiceList, roData);
    });

    connect(invoiceNo, &QComboBox::currentTextChanged, [=](const QString invno){
        auto valueList = io->sql->getInvoiceList(invno.toInt());
        this->setValue(valueList);
    });
}

void AddInvoice::setValue(const QStringList billList)
{

    if(billList.isEmpty())
    {
        clearValue();
        return;
    }
    roNo->setText(billList.at(0));
    roNo->setReadOnly(true);
    invoiceNo->setCurrentText(billList.at(1));
    date->setDate(QDate::fromString(billList.at(2), "dd/MM/yyyy"));
    clients->setCurrentText(billList.at(3));
    clients->setDisabled(true);
    grossAmt->setText(billList.at(4));
    discountPerc->setText(billList.at(5));
    discount->setText(billList.at(6));
    netPayableAmount->setText(billList.at(7));
    cgstPerc->setCurrentText(billList.at(8));
    CGST->setText(billList.at(9));
    sgstPerc->setCurrentText(billList.at(10));
    SGST->setText(billList.at(11));
    igstPerc->setCurrentText(billList.at(12));
    IGST->setText(billList.at(13));
    invoiceAmount->setText(billList.at(14));
    remark->setText(billList.at(15));
}

void AddInvoice::setValueFromRO(const QStringList roList)
{
    clearValue();
    roNo->setText(roList.at(1));
    roNo->setDisabled(true);
    invoiceNo->setCurrentText(QString::number(io->sql->getNewInvoiceCode()));
    date->setDate(QDate());
    clients->setCurrentText(roList.at(6));
    clients->setDisabled(true);
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
            << remark->toPlainText();

    return strList;
}

void AddInvoice::populateData()
{
    clients->addItems(io->dataEngine->clientStringList());
}

void AddInvoice::setValidator()
{
    grossAmt->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    netPayableAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    CGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    SGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    IGST->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    invoiceAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    discount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    discountPerc->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
}

void AddInvoice::clearValue()
{
    roNo->setText("");
    roNo->setDisabled(false);
    invoiceNo->setCurrentText("");
    invoiceNo->setEnabled(false);
    date->setDate(QDate());
    clients->setCurrentText("");
    clients->setEnabled(true);
    grossAmt->setText("");
    discountPerc->setText("");
    discount->setText("");
    netPayableAmount->setText("");
    cgstPerc->setCurrentText("");
    CGST->setText("");
    sgstPerc->setCurrentText("");
    SGST->setText("");
    igstPerc->setCurrentText("");
    IGST->setText("");
    invoiceAmount->setText("");
    remark->setText("");
}


AddInvoice::~AddInvoice()
{

}

void AddInvoice::setValueFromNumbers(const QStringList invnoList)
{
    invoiceNo->addItems(invnoList);
    invoiceNo->setCurrentIndex(0);
}
