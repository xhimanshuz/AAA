#include "UserUI/UserWindow.h"

UserWindow::UserWindow(USER_TYPE _type, QWidget *parent) : QDialog(parent), type(_type)
{
    io = IOHandler::getInstance();
    userMap = (type == USER_TYPE::CLIENT)? io->dataEngine->getClientMap(): io->dataEngine->getMediaHouseMap();

    render();
}

void UserWindow::render()
{
    searchEdit = new QLineEdit(this);
    searchButton = new QPushButton(">>", this);

    usersList = new QTableView(this);
    modelData = new QStandardItemModel(this);
    usersList->setModel(modelData);
    populateData();
    usersList->setSelectionBehavior(QTableView::SelectRows);
    usersList->horizontalHeader()->setStretchLastSection(true);

    newUserWidget = new NewUser(type, this);
    addNew = new QPushButton("Add New", this);
    refreshList = new QPushButton("Refresh", this);
    exit = new QPushButton("Exit", this);

    mainLayout = new QVBoxLayout;

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Search"));
    hbox->addWidget(searchEdit);
    hbox->addWidget(searchButton);
    mainLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(usersList, 2);
    hbox->addWidget(newUserWidget, 1);
    mainLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(addNew);
    hbox->addWidget(refreshList);
    hbox->addWidget(exit);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    setLayout(mainLayout);
    setupSignal();
}

void UserWindow::setupSignal()
{
    connect(newUserWidget->getSaveButton(), &QPushButton::clicked, this, &UserWindow::populateData);

    connect(usersList, &QTableView::clicked, [this](const QModelIndex &index){
        if(index.row() == -1)
            return;

        auto row = modelData->takeRow(index.row());
        newUserWidget->setValues(row);
        modelData->insertRow(index.row(), row);

        populateData();
    });

    connect(refreshList, &QPushButton::clicked, this, &UserWindow::populateData);
}

void UserWindow::populateData()
{
    modelData->clear();
    modelData->setHorizontalHeaderLabels(QStringList()<<"Name"<< "Contact Person"<< "Phone"<< "Email"<< "Address"<< "City"<< "State"<< "GSTIN No."<< "Pin Code");

    for(auto data=userMap->begin(); data!=userMap->end(); ++data )
    {
        modelData->appendRow(QList<QStandardItem*>() << new QStandardItem(data->value(0)) << new QStandardItem(data->value(1)) << new QStandardItem(data->value(2)) << new QStandardItem(data->value(3))
                                << new QStandardItem(data->value(4).replace('\n', ' ')) << new QStandardItem(data->value(5)) << new QStandardItem(data->value(6)) << new QStandardItem(data->value(7))<< new QStandardItem(data->value(8)) );

    }
    usersList->resizeColumnsToContents();

}
