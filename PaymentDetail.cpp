#include "PaymentDetail.h"

PaymentDetail::PaymentDetail(QWidget *parent) : QDialog(parent)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);
}

PaymentDetail::PaymentDetail(int roNo, QWidget *parent):QDialog(parent)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

    QStringList strList = io->dataEngine->paymentStringList(roNo);

    if(!strList.empty())
        setValues(strList);

}

void PaymentDetail::render()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("%0 Details").arg("Payment")));

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

    amountTable = new QTableWidget(1,5);
    amountTable->setHorizontalHeaderLabels(QStringList()<< "Date"<< "Amount"<< "CashCheque"<< "Cheuque No."<< "Bank Name");
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
    form->addRow("Net Amount", netAmount);
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

void PaymentDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        io->dataEngine->insertPaymentData(toStringList());
        this->close();
    });

    connect(amountTable, &QTableWidget::cellChanged, this, &PaymentDetail::cellChanged);
}

void PaymentDetail::setValues(const QStringList paymentStrList)
{
    roNo->setCurrentText(paymentStrList.at(0));
    roNo->setDisabled(true);
    date->setDate(QDate());
    client->setCurrentText(paymentStrList.at(2));
    caption->setText(paymentStrList.at(3));
    dateOfPublicationTelecast->setText(paymentStrList.at(4));
    totalSizeDuration->setText(paymentStrList.at(5));
    premium->setText(paymentStrList.at(6));
    amount->setText(paymentStrList.at(7));
    mediaHouse->setCurrentText(paymentStrList.at(8));
    jobType->setCurrentText(paymentStrList.at(9));
    editionCentre->setText(paymentStrList.at(10));
    sizeDuration->setText(paymentStrList.at(11));
    guarantedPosition->setText(paymentStrList.at(12));
    rate->setText(paymentStrList.at(13));
    netAmount->setText(paymentStrList.at(14));

    auto rowsStr = paymentStrList.at(15).split('\n');
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
    }

    amountTable->setHorizontalHeaderLabels(QStringList()<< "Date"<< "Amount"<< "CashCheque"<< "Cheuque No."<< "Bank Name");

    totalAmount->setText(paymentStrList.at(16));
    balAmount->setText(paymentStrList.at(17));
}

QStringList PaymentDetail::toStringList()
{
    QString rowStr;
    for(auto row=0; row<amountTable->rowCount(); row++)
    {
        auto date = amountTable->item(row, 0);
        auto amount = amountTable->item(row, 1);
        auto cash = amountTable->item(row, 2);
        auto chequeNo = amountTable->item(row, 3);
        auto bankName = amountTable->item(row, 4);
        if(!bankName)
            break;;
        rowStr += date->text() + ','+amount->text()+','+cash->text()+','+ chequeNo->text()+ ',' + bankName->text()+'\n';
    }

    QStringList strList;
    strList << roNo->currentText() << date->text() << client->currentText()<< caption->text()<< dateOfPublicationTelecast->text()
            << totalSizeDuration->text() << premium->text() << amount->text() << mediaHouse->currentText() << jobType->currentText()
            << editionCentre->text() << sizeDuration->text() << guarantedPosition->text() << rate->text() << netAmount->text()
            << rowStr << totalAmount->text() << balAmount->text();

    return strList;
}

void PaymentDetail::tableWidgetSetup()
{
}

void PaymentDetail::cellChanged(int row, int column)
{
    if((!amountTable->item(row, 0) || !amountTable->item(row, 1) || !amountTable->item(row, 2) || !amountTable->item(row, 3) || !amountTable->item(row, 4)) || amountTable->rowCount() != row+1)
        return;

    amountTable->setRowCount(amountTable->rowCount()+1);
}

PaymentDetail::~PaymentDetail()
{

}
