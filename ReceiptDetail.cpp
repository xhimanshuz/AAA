#include "ReceiptDetail.h"



ReceiptDetail::ReceiptDetail(QWidget *parent, int _roNo): rono(_roNo)
{
        io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);



    if(rono != -1)
        setValues();
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

    receiptTableView = new QTableView;
    tableModel = io->sql->getReceiptModel();
    receiptTableView->setModel(tableModel);
    populateTable();
//    receiptTableView->hideRow(0);
//    receiptTableView->hideRow(2);

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

    mainLayout->addWidget(receiptTableView);

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
        io->sql->getReceiptModel()->submitAll();
        this->close();
    });

//    connect(receiptTableView, &QTableWidget::cellChanged, this, &ReceiptDetail::cellChanged);
}

void ReceiptDetail::setValues()
{
    tableModel->setFilter(QString("rono = %0").arg(rono));
    tableModel->select();
}

QStringList ReceiptDetail::toStringList()
{
//    QString rowStr;
//    for(auto row=0; row<receiptTableView->rowCount(); row++)
//    {
//        auto recptNo = receiptTableView->item(row, 0);
//        auto date = receiptTableView->item(row, 1);
//        auto amount = receiptTableView->item(row, 2);
//        auto cash = receiptTableView->item(row, 3);
//        auto chequeNo = receiptTableView->item(row, 4);
//        auto bankName = receiptTableView->item(row, 5);
//        auto remark = receiptTableView->item(row, 6);

//        if(!bankName)
//            break;;
//        rowStr += recptNo->text() +','+ date->text() + ','+ amount->text()+','+cash->text()+','+ chequeNo->text()+ ',' + bankName->text()+ ','+ remark->text()+'\n';
//    }

//    QStringList strList;
//    strList << roNo->currentText() << rowStr;
//    return strList;
}

void ReceiptDetail::populateTable()
{
    io->sql->getReceiptModel()->query().exec();
}

void ReceiptDetail::cellChanged(int row, int column)
{
//    if((!receiptTableView->item(row, 0) || !receiptTableView->item(row, 1) || !receiptTableView->item(row, 2) || !receiptTableView->item(row, 3) || !receiptTableView->item(row, 4)) || receiptTableView->rowCount() != row+1)
//        return;

//    receiptTableView->setRowCount(receiptTableView->rowCount()+1);
}

