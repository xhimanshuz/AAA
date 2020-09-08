#include "PaymentDetail.h"

PaymentDetail::PaymentDetail(QWidget *parent, int roNo) : QDialog(parent), rono(roNo)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);
    QStringList strList = io->dataEngine->paymentStringList(roNo);

//    io->sql->insertBlankRow(rono);

    if(roNo != -1)
        setValues();
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

    paymentTable = new QTableView();
    paymentModel = io->sql->getPaymentModel();
    paymentTable->setModel(paymentModel);
//    paymentTable->hideColumn(0);
    paymentTable->hideColumn(1);
    populateData();

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

    mainLayout->addWidget(paymentTable);

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
    connect(save, &QPushButton::clicked, paymentModel, &QSqlTableModel::submitAll);//    connect(amountTable, &QTableWidget::cellChanged, this, &PaymentDetail::cellChanged);
}

void PaymentDetail::setValues()
{
   paymentModel->setFilter(QString("rono = %0").arg(rono));
   paymentModel->select();
}

QStringList PaymentDetail::toStringList()
{
//    QString rowStr;
//    for(auto row=0; row<amountTable->rowCount(); row++)
//    {
//        auto date = amountTable->item(row, 0);
//        auto amount = amountTable->item(row, 1);
//        auto cash = amountTable->item(row, 2);
//        auto chequeNo = amountTable->item(row, 3);
//        auto bankName = amountTable->item(row, 4);
//        if(!bankName)
//            break;;
//        rowStr += date->text() + ','+amount->text()+','+cash->text()+','+ chequeNo->text()+ ',' + bankName->text()+'\n';
//    }

//    QStringList strList;
//    strList << roNo->currentText()
//            << rowStr;

//    return strList;
}

void PaymentDetail::populateData()
{
    paymentModel->query().exec();
    paymentTable->resizeColumnsToContents();
    paymentTable->viewport()->update();
}

void PaymentDetail::cellChanged(int row, int column)
{

}

PaymentDetail::~PaymentDetail()
{

}
