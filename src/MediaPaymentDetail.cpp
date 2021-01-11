#include "MediaPaymentDetail.h"
#include "pdftroninterface.h"

MediaPaymentDetail::MediaPaymentDetail(QWidget *parent, int roNo) : QDialog(parent), rono(roNo), io{IOHandler::getInstance()}, latestMediaPaymentId{io->sql->getNewMediaPaymentNumber()}
{
    render();
    setLayout(mainLayout);
    QStringList strList = io->dataEngine->paymentStringList(roNo);

    if(roNo > 0)
        setValues();
}

void MediaPaymentDetail::render()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("%0 Details").arg("Payment")));

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
    netAmount = new QLineEdit(this);
    roAmount = new QLineEdit(this);

    paymentTable = new QTableWidget(0, 8);
    insertNewRow();
    paymentTable->setHorizontalHeaderLabels(QStringList()<< "ID"<< "Date"<< "Amount"<<"Mode"<<"Cheque No."<<"Bank Name"<<" "<<" ");
    paymentTable->setItemDelegateForColumn(0, new CustomItemDelegate::Label(this));
    paymentTable->setItemDelegateForColumn(1, new CustomItemDelegate::Date(this));
    paymentTable->setItemDelegateForColumn(2, new CustomItemDelegate::Amount(this));
    paymentTable->setItemDelegateForColumn(3, new CustomItemDelegate::Mode(this));

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
    form->addRow("RO Amount", roAmount);
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
    setValidator();

}

void MediaPaymentDetail::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to Save Changes", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        io->sql->insertMediaPayment(toStringList(), rono);
    });

    connect(paymentTable, &QTableWidget::cellChanged, this, &MediaPaymentDetail::cellChanged);
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

void MediaPaymentDetail::setValues()
{
    roNo->setText(QString::number(rono));
//    roNo->setDisabled(true);

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
    roAmount->setText(strList.at(34));
    roAmount->setReadOnly(true);

    auto list = io->sql->getMediaPaymentStringListByRono(rono);
    populateData(list);
}


QList<QStringList> MediaPaymentDetail::toStringList()
{
    QList<QStringList> list;
    for(auto r=0; r< paymentTable->rowCount()-1; r++)
    {
        if(!paymentTable->item(r, 1) && !paymentTable->item(r, 2) && !paymentTable->item(r, 3) && !paymentTable->item(r, 4))
            continue;
        QStringList strList;
        for(auto c=0; c< paymentTable->columnCount()-2; c++)
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


void MediaPaymentDetail::populateData(QList<QStringList> list)
{
    for(auto r=0; r< list.size(); r++)
    {
        auto l = list.at(r);

        for(auto c=0; c<list.at(r).size(); c++)
        {
            paymentTable->setItem(r, c, new QTableWidgetItem(l.at(c)));
        }
        qobject_cast<QPushButton*>(paymentTable->cellWidget(r, 6))->setEnabled(true);
    }
}

void MediaPaymentDetail::insertNewRow()
{
    paymentTable->setRowCount(paymentTable->rowCount()+1);

    latestMediaPaymentId += 1;
    paymentTable->setItem(paymentTable->rowCount()-1, 0, new QTableWidgetItem(QString::number(latestMediaPaymentId)));
    if(paymentTable->rowCount() > 2)
        qobject_cast<QPushButton*>(paymentTable->cellWidget(paymentTable->rowCount()-2, 6))->setEnabled(true);
    paymentTable->setCellWidget(paymentTable->rowCount()-1, 6, createPrintButton());
    paymentTable->setCellWidget(paymentTable->rowCount()-1, 7, createDeleteButton());
    paymentTable->setItem(paymentTable->rowCount()-1, 1, new QTableWidgetItem(QDate::currentDate().toString("d/MM/yyyy")));

    paymentTable->viewport()->update();
}

void MediaPaymentDetail::setValidator()
{
    amount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    premium->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    rate->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    netAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    totalAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    balAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
}

QPushButton *MediaPaymentDetail::createPrintButton()
{
    QPushButton *cellPrintButton = new QPushButton("Print");
    connect(cellPrintButton, &QPushButton::clicked, this, &MediaPaymentDetail::printMediaPayment);
    cellPrintButton->setDisabled(true);
    return cellPrintButton;
}

QPushButton *MediaPaymentDetail::createDeleteButton()
{
    QPushButton *cellPrintButton = new QPushButton("Delete");
    connect(cellPrintButton, &QPushButton::clicked, [this]{
        auto row = paymentTable->currentRow();
        if(!isValidRow(row))
            return;
        paymentTable->removeRow(row);
//        auto id = paymentTable->item(row, 1)->text();

//        auto mpNo = io->sql->getMediaPaymentStringList(id.toInt());
        if(paymentTable->rowCount() < 1)
            insertNewRow();

    });

    return cellPrintButton;
}

QLineEdit *MediaPaymentDetail::createLineEditNumber(const QString amount)
{
    auto lineEdit = new QLineEdit(amount);
    lineEdit->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));

    return lineEdit;
}

bool MediaPaymentDetail::isValidRow(const int row)
{
    if(!paymentTable->item(row, 0) || !paymentTable->item(row, 1) || !paymentTable->item(row, 2) || !paymentTable->item(row, 3) || !paymentTable->item(row, 4) )
        return false;
    return true;

}

void MediaPaymentDetail::setTotalAmount()
{
    auto totalAmountValue{0};
    auto cr = paymentTable->rowCount();
    for(auto row = 0; row < paymentTable->rowCount()-1; row++)
    {
        auto value = paymentTable->item(row, 2)->text().toInt();
        totalAmountValue += value;
    }
    auto na = roAmount->text().toDouble();
    totalAmount->setText(QString::number(totalAmountValue));
    balAmount->setText(QString::number(totalAmountValue - roAmount->text().toDouble()));
}

void MediaPaymentDetail::printMediaPayment()
{
    auto id = paymentTable->item(paymentTable->currentIndex().row(), 0)->text().toInt();
    auto roList = io->sql->getROStringList(rono);
    auto mediaPaymentList = io->sql->getMediaPaymentStringList(id);
    auto mediaHouseList = io->sql->getMediaHouseRow(roList.at(3).toInt());
    roList << mediaHouseList;
    if(!mediaPaymentList.isEmpty())
        PDFTronInterface::get()->printRO(roList, mediaPaymentList);
}

void MediaPaymentDetail::cellChanged(int row, int column)
{
    setTotalAmount();

    if(!isValidRow(row))
        return;


    if(row+1 != paymentTable->rowCount())
        return;
//    paymentTable->item(paymentTable->row())
    insertNewRow();
}

MediaPaymentDetail::~MediaPaymentDetail()
{

}
