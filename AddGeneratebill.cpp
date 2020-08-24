#include "AddGeneratebill.h"

AddGenerateBill::AddGenerateBill(QWidget *parent, const int roNo, const QStringList billData): QWidget(parent)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

    if(!billData.isEmpty())
        setValue(billData);
}

void AddGenerateBill::render()
{

    roNo = new QLineEdit;
    invoiceNo = new QLineEdit;
    date = new QDateEdit;
    clients = new QComboBox();

    grossAmt = new QLineEdit;
    discount = new QLineEdit;
    discountPerc = new QLineEdit;
    netPayableAmount = new QLineEdit;
    cgstPerc = new QComboBox;
    CGST = new QLineEdit;
    sgstPerc = new QComboBox;
    SGST = new QLineEdit;
    igstPerc = new QComboBox;
    IGST = new QLineEdit;
    invoiceAmount = new QLineEdit;
    remark = new QTextEdit;
    totSizeDuration = new QListView;
    save = new QPushButton("Save");
    remove = new QPushButton("Remove");
    clear = new QPushButton("Clear");

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
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    populateData();
    setupSignal();
}

void AddGenerateBill::setupSignal()
{
    connect(save, &QPushButton::clicked, [this]{
        io->dataEngine->insertGBillData(toStringList());
        emit saveClicked();
    });
}

void AddGenerateBill::setValue(const QStringList billList)
{
    roNo->setText(billList.at(0));
    roNo->setDisabled(true);
    invoiceNo->setText(billList.at(1));
    invoiceNo->setEnabled(false);
    date->setDate(QDate());
    clients->setCurrentText(billList.at(3));
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

QStringList AddGenerateBill::toStringList()
{
    QStringList strList;
    strList << roNo->text()<< invoiceNo->text()<< date->text() << clients->currentText()<< grossAmt->text() << discountPerc->text()<< netPayableAmount->text()<< cgstPerc->currentText()<<CGST->text()
            << sgstPerc->currentText()<< SGST->text() << igstPerc->currentText()<< IGST->text()<< invoiceAmount->text()<< remark->toPlainText();

    return strList;
}

void AddGenerateBill::populateData()
{
    clients->addItems(io->dataEngine->clientStringList());
}


AddGenerateBill::~AddGenerateBill()
{

}
