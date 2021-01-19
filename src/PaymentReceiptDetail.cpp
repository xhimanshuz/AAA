#include "PaymentReceiptDetail.h"
#include "pdftroninterface.h"
#include "CustomItemDelegate.h"

PaymentReceiptDetail::PaymentReceiptDetail(QWidget *parent, int _roNo): QDialog(parent), rono(_roNo), io{IOHandler::getInstance()}, latestInvoiceNo{io->sql->getNewPaymentReceiptNumber()}
{
    render();
    setLayout(mainLayout);

    if(rono > 0)
        setValues();
}


PaymentReceiptDetail::~PaymentReceiptDetail()
{

}

void PaymentReceiptDetail::render()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("%0 Details").arg("Receipt")));

    roNo = new QLineEdit;
    roNo->setReadOnly(true);
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

    receiptTableWidget = new QTableWidget(0, 9);
    insertNewRow();
    receiptTableWidget->setHorizontalHeaderLabels(QStringList() << "Rcpt. No." << "Date"<<"Amount"<<"Mode"<<"Cheque No."<<"Bank Name"<<"Remark");
    receiptTableWidget->setItemDelegateForColumn(0, new CustomItemDelegate::Label(this));
    receiptTableWidget->setItemDelegateForColumn(1, new CustomItemDelegate::Date(this));
    receiptTableWidget->setItemDelegateForColumn(2, new CustomItemDelegate::Amount(this));
    receiptTableWidget->setItemDelegateForColumn(3, new CustomItemDelegate::Mode(this));

    totalAmount = new QLineEdit;
    balAmount = new QLineEdit;
    save = new QPushButton("Save");
    clear = new QPushButton("Clear");
//    print = new QPushButton("Print");

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

void PaymentReceiptDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to Save Changes", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        io->sql->insertPaymentReceipt(toStringList(), rono);
    });

    connect(receiptTableWidget, &QTableWidget::cellChanged, this, &PaymentReceiptDetail::cellChanged);
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

//    connect(print, &QPushButton::clicked, [this]{

//        for(auto list: toStringList())
//        {
//            QStringList roDetail = QStringList()<< QString::number(rono) << client->currentText() <<
//                                                   mediaHouse->currentText()<< "03-11-2020"<< "09AIIPR8398A1ZQ"<< amount->text();
//            PDFTronInterface::get()->printReceipt(list, roDetail);
//        }
//    });
}

void PaymentReceiptDetail::setValues()
{
    roNo->setText(QString::number(rono));

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

    auto list = io->sql->getPaymentReceiptStringListByRO(rono);
    populateTable(list);
}

QList<QStringList> PaymentReceiptDetail::toStringList()
{
    QList<QStringList> list;
//    auto rowCount = receiptTableWidget->rowCount();
//    auto columnCount = receiptTableWidget->columnCount();

    for(auto r=0; r< receiptTableWidget->rowCount()-1; r++)
    {
        if(!receiptTableWidget->item(r, 1) && !receiptTableWidget->item(r, 2) && !receiptTableWidget->item(r, 3) && !receiptTableWidget->item(r, 4))
            continue;
        QStringList strList;
        for(auto c=0; c< receiptTableWidget->columnCount()-2; c++)
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

void PaymentReceiptDetail::populateTable(QList<QStringList> list)
{
    for(auto r=0; r<list.size(); r++)
    {
        auto l = list.at(r);

        for(auto c=0; c<list.at(r).size(); c++)
        {
            receiptTableWidget->setItem(r, c, new QTableWidgetItem(l.at(c)));
        }
        qobject_cast<QPushButton*>(receiptTableWidget->cellWidget(r, 7))->setEnabled(true);
    }
}

void PaymentReceiptDetail::insertNewRow()
{
    receiptTableWidget->setRowCount(receiptTableWidget->rowCount()+1);

    latestInvoiceNo +=1;
    receiptTableWidget->setItem(receiptTableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(latestInvoiceNo)));
    if(receiptTableWidget->rowCount()>2)
        qobject_cast<QPushButton*>(receiptTableWidget->cellWidget(receiptTableWidget->rowCount()-2, 7))->setEnabled(true);
    receiptTableWidget->setCellWidget(receiptTableWidget->rowCount()-1, 7, createPrintButton());
    receiptTableWidget->setCellWidget(receiptTableWidget->rowCount()-1, 8, createDeleteButton());
    receiptTableWidget->setItem(receiptTableWidget->rowCount()-1, 1, new QTableWidgetItem(QDate::currentDate().toString("d/MM/yyyy")));

    receiptTableWidget->viewport()->update();
}

void PaymentReceiptDetail::setValidator()
{
    amount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    premium->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    rate->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    netAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    totalAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    balAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
}

QPushButton *PaymentReceiptDetail::createPrintButton()
{
    QPushButton *cellPrintButton = new QPushButton("Print");
    connect(cellPrintButton, &QPushButton::clicked, [this]{
       auto id = receiptTableWidget->item(receiptTableWidget->currentIndex().row(), 0)->text().toInt();
//       auto roList = io->sql->getROStringList();
       QStringList roDetail = QStringList()<< QString::number(rono) << client->currentText() <<
                                              mediaHouse->currentText()<< "03-11-2020"<< "09AIIPR8398A1ZQ"<< amount->text();
       auto number = receiptTableWidget->item(receiptTableWidget->currentIndex().row(), 0)->text().toInt();
       auto paymetReceiptList = io->sql->getPaymentReceiptStringList(number);
       PDFTronInterface::get()->printReceipt(paymetReceiptList, roDetail);
    });
    cellPrintButton->setDisabled(true);
    return cellPrintButton;
}

QPushButton *PaymentReceiptDetail::createDeleteButton()
{
    QPushButton *cellPrintButton = new QPushButton("Delete");
    connect(cellPrintButton, &QPushButton::clicked, [this]{
        auto row = receiptTableWidget->currentRow();
        if(!isValidRow(row))
            return;
        receiptTableWidget->removeRow(row);
//        auto id = paymentTable->item(row, 1)->text();

//        auto mpNo = io->sql->getMediaPaymentStringList(id.toInt());
        if(receiptTableWidget->rowCount() < 1)
            insertNewRow();

    });

    return cellPrintButton;
}

bool PaymentReceiptDetail::isValidRow(int row)
{
    if(!receiptTableWidget->item(row, 1) || !receiptTableWidget->item(row, 2) || !receiptTableWidget->item(row, 3) || !receiptTableWidget->item(row, 4) || !receiptTableWidget->item(row, 5) )
        return false;
    return true;
}

void PaymentReceiptDetail::setTotalAmount()
{
    auto totalAmountValue{0};
    auto cr = receiptTableWidget->rowCount();
    for(auto row = 0; row < receiptTableWidget->rowCount()-1; row++)
    {
        auto value = receiptTableWidget->item(row, 2)->text().toInt();
        totalAmountValue += value;
    }

    auto na = netAmount->text().toDouble();
    totalAmount->setText(QString::number(totalAmountValue));
    balAmount->setText(QString::number(netAmount->text().toDouble()-totalAmountValue));
}

void PaymentReceiptDetail::cellChanged(int row, int column)
{
    setTotalAmount();

    if(!isValidRow(row))
        return;

    if(row+1 != receiptTableWidget->rowCount())
        return;

    qobject_cast<QPushButton*>(receiptTableWidget->cellWidget(receiptTableWidget->rowCount()-1, 7))->setEnabled(true);
    insertNewRow();
}

