#include "MediaBillDetail.h"


//MediaBill::MediaBill(QWidget *parent):QDialog(parent)
//{
//    io = IOHandler::getInstance();

//    render();
//    setLayout(mainLayout);
//}

MediaBill::MediaBill(QWidget *parent, const int _roNo):QDialog(parent), rono(_roNo)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

    if(rono != -1)
        setValues();
}

void MediaBill::render()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("%0 Details").arg("Media Bill")));

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

    mediaBillTableView = new QTableView;
    mediaBillModel = io->sql->getMediaBill();
    mediaBillTableView->setModel(mediaBillModel);
    populateData();

    totalAmount = new QLineEdit;
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

    mainLayout->addWidget(mediaBillTableView);

    form = new QFormLayout;
    form->addRow("Total Amount", totalAmount);
    mainLayout->addLayout(form);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    setupSignals();
}

void MediaBill::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        io->dataEngine->insertMediaBillData(toStringList());
        this->close();
    });

//    connect(mediaBillTableView, &QTableWidget::cellChanged, this, &MediaBill::cellChanged);
}

void MediaBill::setValues()
{
    roNo->setCurrentText(QString::number(rono));
    roNo->setDisabled(true);

    mediaBillModel->setFilter(QString("rono = %0").arg(rono));
    mediaBillModel->select();
//    date->setDate(QDate());
//    client->setCurrentText(paymentStrList.at(2));
//    caption->setText(paymentStrList.at(3));
//    dateOfPublicationTelecast->setText(paymentStrList.at(4));
//    totalSizeDuration->setText(paymentStrList.at(5));
//    premium->setText(paymentStrList.at(6));
//    amount->setText(paymentStrList.at(7));
//    mediaHouse->setCurrentText(paymentStrList.at(8));
//    jobType->setCurrentText(paymentStrList.at(9));
//    editionCentre->setText(paymentStrList.at(10));
//    sizeDuration->setText(paymentStrList.at(11));
//    guarantedPosition->setText(paymentStrList.at(12));
//    rate->setText(paymentStrList.at(13));
//    netAmount->setText(paymentStrList.at(14));

//    auto rowsStr = paymentStrList.at(1).split('\n');
//    mediaBillTableView->clear();
//    for(auto r=0; r< rowsStr.size()-1; r++)
//    {
//        mediaBillTableView->setRowCount(r+1);
//        auto row = rowsStr.at(r).split(',');
//        mediaBillTableView->setItem(r, 0, new QTableWidgetItem(row.at(0)));
//        mediaBillTableView->setItem(r, 1, new QTableWidgetItem(row.at(1)));
//        mediaBillTableView->setItem(r, 2, new QTableWidgetItem(row.at(2)));
//    }

//    mediaBillTableView->setHorizontalHeaderLabels(QStringList()<< "Media Bill"<< "Date"<< "Amount");

//    totalAmount->setText(paymentStrList.at(16));
}

QStringList MediaBill::toStringList()
{
//    QString rowStr;
//    for(auto row=0; row<mediaBillTableView->rowCount(); row++)
//    {
//        auto mediaBillNo = mediaBillTableView->item(row, 0);
//        auto date = mediaBillTableView->item(row, 1);
//        auto amount = mediaBillTableView->item(row, 2);
//        if(!amount)
//            break;;
//        rowStr += mediaBillNo->text()+','+ date->text() + ','+amount->text()+'\n';
//    }

//    QStringList strList;

//    strList << roNo->currentText() << rowStr;

//    return strList;
}

void MediaBill::populateData()
{
    mediaBillModel->query().exec();
}

void MediaBill::cellChanged(int row, int column)
{
//    if((!mediaBillTableView->item(row, 0) || !mediaBillTableView->item(row, 1) || !mediaBillTableView->item(row, 2) ) || mediaBillTableView->rowCount() != row+1)
//        return;

//    mediaBillTableView->setRowCount(mediaBillTableView->rowCount()+1);
}
