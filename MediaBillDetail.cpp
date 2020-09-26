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

    mediaBillTableWidget = new QTableWidget(1, 3, this);
    mediaBillTableWidget->setHorizontalHeaderLabels(QStringList()<< "MB. No."<<"Date"<<"Amount");
//    mediaBillSModel = new QStandardItemModel;
//    mediaBillModel = io->sql->getMediaBill();

//    mediaBillTableView->setModel(mediaBillSModel);
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

    mainLayout->addWidget(mediaBillTableWidget);

    form = new QFormLayout;
    form->addRow("Total Amount", totalAmount);
    mainLayout->addLayout(form);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    deleteRow = new QAction;
    deleteRow->setShortcut(Qt::Key::Key_Delete);
    mediaBillTableWidget->addAction(deleteRow);

    setupSignals();
}

void MediaBill::setupSignals()
{
    connect(save, &QPushButton::clicked, [this]{
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to Save Changes", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        io->sql->insertMediaBill(toStringList(), rono);
    });

    connect(mediaBillTableWidget, &QTableWidget::cellChanged, this, &MediaBill::cellChanged);

    connect(deleteRow, &QAction::triggered, [this]()
    {
        QMessageBox msg(QMessageBox::Icon::Warning, "Warning!", "Do you want to delete", QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes !=  msg.exec())
            return;
        auto row = mediaBillTableWidget->currentRow();
        if(!mediaBillTableWidget->item(1, 0) && !mediaBillTableWidget->item(1, 1) && !mediaBillTableWidget->item(1, 2) && (row == 1))
            return;

        mediaBillTableWidget->removeRow(row);
        if(mediaBillTableWidget->rowCount() < 1)
            mediaBillTableWidget->setRowCount(1);
    });
}

void MediaBill::setValues()
{
    roNo->setCurrentText(QString::number(rono));
    roNo->setDisabled(true);

    auto list = io->sql->getMediaBillList(rono);

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

QList<QStringList> MediaBill::toStringList()
{
    QList<QStringList> list;
    for(auto r=0; r< mediaBillTableWidget->rowCount()-1; r++)
    {
        QStringList strList;
        for(auto c=0; c< mediaBillTableWidget->columnCount(); c++)
        {
            if(mediaBillTableWidget->item(r, c))
                strList << mediaBillTableWidget->item(r, c)->text();
            else
                strList << "";
        }
        list << strList;
    }

    return list;
}

void MediaBill::populateData(QList<QStringList> list)
{
    for(auto r=0; r< list.size(); r++)
    {
        for(auto c=0; c<list.at(r).size(); c++)
        {
            auto l = list.at(r);
            mediaBillTableWidget->setItem(r, c, new QTableWidgetItem(l.at(c)));
        }
     }

}

void MediaBill::insertNewRow()
{
    mediaBillTableWidget->setRowCount(mediaBillTableWidget->rowCount()+1);
}

void MediaBill::cellChanged(int row, int column)
{
    if(!mediaBillTableWidget->item(row, 0) || !mediaBillTableWidget->item(row, 1) ||!mediaBillTableWidget->item(row, 2))
        return;

    if(column+1 != mediaBillTableWidget->columnCount())
        return;

    insertNewRow();
}
