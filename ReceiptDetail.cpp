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
    client->addItems(io->sql->getClientList());
    caption = new QLineEdit;
    dateOfPublicationTelecast = new QLineEdit;
    totalSizeDuration = new QLineEdit;
    premium = new QLineEdit;
    amount = new QLineEdit;
    mediaHouse = new QComboBox;
    mediaHouse->addItems(io->sql->getMediaHouseList());
    jobType = new QComboBox;
    jobType->addItems(io->sql->getJobTypeList());
    editionCentre = new QLineEdit;
    sizeDuration = new QLineEdit;
    guarantedPosition = new QLineEdit;
    rate = new QLineEdit;
    netAmount = new QLineEdit;

    receiptTableWidget = new QTableWidget(1, 6);
    receiptTableWidget->setHorizontalHeaderLabels(QStringList() << "Date"<<"Amount"<<"Mode"<<"Cheque No."<<"Bank Name"<<"Remark");
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

    mainLayout->addWidget(receiptTableWidget);

    form = new QFormLayout;
    form->addRow("Total Amount", totalAmount);
    form->addRow("Bal Amount", balAmount);
    mainLayout->addLayout(form);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    deleteRow = new QAction();
    deleteRow->setShortcut(Qt::Key::Key_Delete);
    receiptTableWidget->addAction(deleteRow);

    setupSignals();
}

void ReceiptDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to Save Changes", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        io->sql->insertReceipt(toStringList(), rono);
    });

    connect(receiptTableWidget, &QTableWidget::cellChanged, this, &ReceiptDetail::cellChanged);
    connect(deleteRow, &QAction::triggered, [this]()
    {
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to delete", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        auto row = receiptTableWidget->currentRow();
        if(!receiptTableWidget->item(1, 0) && !receiptTableWidget->item(1, 1) && !receiptTableWidget->item(1, 2) && !receiptTableWidget->item(1, 3) && !receiptTableWidget->item(1, 4) && (row == 1))
        {
            return;
        }
        receiptTableWidget->removeRow(row);
        if(receiptTableWidget->rowCount() < 1)
            receiptTableWidget->setRowCount(1);
    });
}

void ReceiptDetail::setValues()
{
    roNo->setCurrentText(QString::number(rono));

    auto list = io->sql->getReceiptStringList(rono);

    populateTable(list);

    auto strList = io->sql->getROStringList(rono);
    date->setDate(QDate());
    date->setReadOnly(true);
    client->setCurrentText(strList.at(6));
    client->setDisabled(true);
    caption->setText(strList.at(9));
    caption->setReadOnly(true);
    dateOfPublicationTelecast->setText(strList.at(11));
    dateOfPublicationTelecast->setReadOnly(true);
    totalSizeDuration->setText(strList.at(13));
    totalSizeDuration->setReadOnly(true);
    premium->setText(strList.at(15));
    premium->setReadOnly(true);
    amount->setText(strList.at(19));
    amount->setReadOnly(true);
    mediaHouse->setCurrentText(strList.at(4));
    mediaHouse->setDisabled(true);
    jobType->setCurrentText(strList.at(8));
    jobType->setDisabled(true);
    editionCentre->setText(strList.at(10));
    editionCentre->setReadOnly(true);
    sizeDuration->setText(strList.at(12));
    sizeDuration->setReadOnly(true);
    guarantedPosition->setText(strList.at(14));
    guarantedPosition->setReadOnly(true);
    rate->setText(strList.at(17));
    rate->setReadOnly(true);
    netAmount->setText(strList.at(20));
    netAmount->setReadOnly(true);
}

QList<QStringList> ReceiptDetail::toStringList()
{
    QList<QStringList> list;
    for(auto r=0; r< receiptTableWidget->rowCount()-1; r++)
    {
        QStringList strList;
        for(auto c=0; c< receiptTableWidget->columnCount(); c++)
        {
            if(receiptTableWidget->item(r, c))
                strList << receiptTableWidget->item(r, c)->text();
            else
                strList << "";
        }
        list << strList;
    }

    return list;
}

void ReceiptDetail::populateTable(QList<QStringList> list)
{
    for(auto r=0; r< list.size(); r++)
    {
        auto l = list.at(r);
        for(auto c=0; c<list.at(r).size(); c++)
        {
            receiptTableWidget->setItem(r, c, new QTableWidgetItem(l.at(c)));
        }
    }
}

void ReceiptDetail::insertNewRow()
{
    receiptTableWidget->setRowCount(receiptTableWidget->rowCount()+1);
    receiptTableWidget->viewport()->update();
}

void ReceiptDetail::cellChanged(int row, int column)
{
    if(!receiptTableWidget->item(row, 0) || !receiptTableWidget->item(row, 1) || !receiptTableWidget->item(row, 2) || !receiptTableWidget->item(row, 3) || !receiptTableWidget->item(row, 4) )
        return;

    if(row+1 != receiptTableWidget->rowCount())
        return;

    insertNewRow();
}

