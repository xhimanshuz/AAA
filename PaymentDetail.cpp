#include "PaymentDetail.h"

PaymentDetail::PaymentDetail(QWidget *parent, int roNo) : QDialog(parent), rono(roNo)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);
    QStringList strList = io->dataEngine->paymentStringList(roNo);

//    io->sql->insertBlankRow(rono);

    if(roNo > 0)
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

    paymentTable = new QTableWidget(1, 5);
    paymentTable->setHorizontalHeaderLabels(QStringList()<< "Date"<< "Amount"<<"Mode"<<"Cheque No."<<"Bank Name");

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

    deleteRow = new QAction;
    deleteRow->setShortcut(Qt::Key::Key_Delete);
    paymentTable->addAction(deleteRow);

    setupSignals();
}

void PaymentDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to Save Changes", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        io->sql->insertPayment(toStringList(), rono);
    });

    connect(paymentTable, &QTableWidget::cellChanged, this, &PaymentDetail::cellChanged);
    connect(deleteRow, &QAction::triggered, [this]()
    {
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to delete", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        auto row = paymentTable->currentRow();
        if(!paymentTable->item(1, 0) && !paymentTable->item(1, 1) && !paymentTable->item(1, 2) && !paymentTable->item(1, 3) && !paymentTable->item(1, 4) && (row == 1))
            return;

        paymentTable->removeRow(row);
        if(paymentTable->rowCount() < 1)
            paymentTable->setRowCount(1);
    });
}

void PaymentDetail::setValues()
{
    roNo->setCurrentText(QString::number(rono));
    roNo->setDisabled(true);

    auto list = io->sql->getPaymentStringList(rono);

    populateData(list);

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

QList<QStringList> PaymentDetail::toStringList()
{
    QList<QStringList> list;
    for(auto r=0; r< paymentTable->rowCount()-1; r++)
    {
        QStringList strList;
        for(auto c=0; c< paymentTable->columnCount(); c++)
        {
            if(paymentTable->item(r, c))
                strList << paymentTable->item(r, c)->text();
            else
                strList << "";
        }
        list << strList;
    }

    return list;
}

void PaymentDetail::populateData(QList<QStringList> list)
{
    for(auto r=0; r< list.size(); r++)
    {
        auto l = list.at(r);
        for(auto c=0; c<list.at(r).size(); c++)
        {
            paymentTable->setItem(r, c, new QTableWidgetItem(l.at(c)));
        }
    }
}

void PaymentDetail::insertNewRow()
{
    paymentTable->setRowCount(paymentTable->rowCount()+1);
    paymentTable->viewport()->update();
}

void PaymentDetail::cellChanged(int row, int column)
{
    if(!paymentTable->item(row, 0) || !paymentTable->item(row, 1) || !paymentTable->item(row, 2) || !paymentTable->item(row, 3) || !paymentTable->item(row, 4) )
        return;

    if(row+1 != paymentTable->rowCount())
        return;

    insertNewRow();
}

PaymentDetail::~PaymentDetail()
{

}
