#include "ReceiptDetail.h"



ReceiptDetail::ReceiptDetail(QWidget *parent, int roNo)
{
        io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

    auto strList = io->dataEngine->receiptStringList(roNo);

    if(!strList.empty())
        setValues(strList);
}


ReceiptDetail::~ReceiptDetail()
{

}

void ReceiptDetail::render()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("%0 Details").arg("Receipt")));

    roNo = new QComboBox;
    roNo->setEditable(true);
    date = new QDateEdit;
    client = new QComboBox;
    client->addItems(io->dataEngine->clientStringList());
    caption = new QLineEdit;
    dateOfPublicationTelecast = new QLineEdit;
    totalSizeDuration = new QLineEdit;
    premium = new QLineEdit;
    amount = new QLineEdit;
    mediaHouse = new QComboBox;
    mediaHouse->addItems(io->dataEngine->mediaHouseStringList());
    jobType = new QComboBox;
    jobType->addItems(io->dataEngine->jobTypeStringList());
    editionCentre = new QLineEdit;
    sizeDuration = new QLineEdit;
    guarantedPosition = new QLineEdit;
    rate = new QLineEdit;
    netAmount = new QLineEdit;

    amountTable = new QTableWidget(1,7);

    tableWidgetSetup();

    totalAmount = new QLineEdit;
    balAmount = new QLineEdit;
    save = new QPushButton("Save");
    clear = new QPushButton("Clear");

    QHBoxLayout *formHbox = new QHBoxLayout;
    QFormLayout *form = new QFormLayout;


    form->addRow("RO No", roNo);
    form->addRow("Date", date);
    form->addRow("Clients", client);
    form->addRow("Caption", caption);
    form->addRow("Date of Publication/Telecast", dateOfPublicationTelecast);
    form->addRow("Total Size/Duration", totalSizeDuration);
    form->addRow("Premium", premium);
    form->addRow("Amount", amount);
    formHbox->addLayout(form);

    form = new QFormLayout;
    form->addRow("Media House", mediaHouse);
    form->addRow("Job Type", jobType);
    form->addRow("Edition/Centre", editionCentre);
    form->addRow("Size/Duration", sizeDuration);
    form->addRow("Guaranted Position", guarantedPosition);
    form->addRow("Rate", rate);
    form->addRow("Bill Amount", netAmount);
    formHbox->addLayout(form);
    mainLayout->addLayout(formHbox);

    mainLayout->addWidget(amountTable);

    form = new QFormLayout;
    form->addRow("Total Amount", totalAmount);
    form->addRow("Bal Amount", balAmount);
    mainLayout->addLayout(form);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    setupSignals();
}

void ReceiptDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        io->dataEngine->insertReceiptData(toStringList());
        this->close();
    });

    connect(amountTable, &QTableWidget::cellChanged, this, &ReceiptDetail::cellChanged);
}

void ReceiptDetail::setValues(const QStringList paymentStrList)
{
    roNo->setCurrentText(paymentStrList.at(0));
    roNo->setEnabled(false);

    auto rowsStr = paymentStrList.at(1).split('\n');
    amountTable->clear();
    for(auto r=0; r< rowsStr.size()-1; r++)
    {
        amountTable->setRowCount(r+1);
        auto row = rowsStr.at(r).split(',');
        amountTable->setItem(r, 0, new QTableWidgetItem(row.at(0)));
        amountTable->setItem(r, 1, new QTableWidgetItem(row.at(1)));
        amountTable->setItem(r, 2, new QTableWidgetItem(row.at(2)));
        amountTable->setItem(r, 3, new QTableWidgetItem(row.at(3)));
        amountTable->setItem(r, 4, new QTableWidgetItem(row.at(4)));
        amountTable->setItem(r, 5, new QTableWidgetItem(row.at(5)));
        amountTable->setItem(r, 6, new QTableWidgetItem(row.at(6)));
    }

    amountTable->setHorizontalHeaderLabels(QStringList()<< "Recpt No." "Date"<< "Amount"<< "CashCheque"<< "Cheuque No."<< "Bank Name"<<"Remark"<< "Print");

}

QStringList ReceiptDetail::toStringList()
{
    QString rowStr;
    for(auto row=0; row<amountTable->rowCount(); row++)
    {
        auto recptNo = amountTable->item(row, 0);
        auto date = amountTable->item(row, 1);
        auto amount = amountTable->item(row, 2);
        auto cash = amountTable->item(row, 3);
        auto chequeNo = amountTable->item(row, 4);
        auto bankName = amountTable->item(row, 5);
        auto remark = amountTable->item(row, 6);

        if(!bankName)
            break;;
        rowStr += recptNo->text() +','+ date->text() + ','+ amount->text()+','+cash->text()+','+ chequeNo->text()+ ',' + bankName->text()+ ','+ remark->text()+'\n';
    }

    QStringList strList;
    strList << roNo->currentText() << rowStr;
    return strList;
}

void ReceiptDetail::tableWidgetSetup()
{
    amountTable->setHorizontalHeaderLabels(QStringList()<< "Recpt No." "Date"<< "Amount"<< "CashCheque"<< "Cheuque No."<< "Bank Name"<<"Remark"<< "Print");
}

void ReceiptDetail::cellChanged(int row, int column)
{
    if((!amountTable->item(row, 0) || !amountTable->item(row, 1) || !amountTable->item(row, 2) || !amountTable->item(row, 3) || !amountTable->item(row, 4)) || amountTable->rowCount() != row+1)
        return;

    amountTable->setRowCount(amountTable->rowCount()+1);
}

