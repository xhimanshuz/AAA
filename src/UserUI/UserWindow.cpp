#include "UserUI/UserWindow.h"
#include <QDebug>

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
    modelData = (type == USER_TYPE::MEDIA_HOUSE)?io->sql->getMediaHouseModel():io->sql->getClietModel();
    usersList->setModel(modelData);
    usersList->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
    usersList->setSelectionMode(QTableView::SelectionMode::SingleSelection);
    populateData();
    usersList->setSelectionBehavior(QTableView::SelectRows);
    usersList->horizontalHeader()->setStretchLastSection(true);
//    usersList->setSi

    newUserWidget = new NewUser(type, this);
    addNew = new QPushButton("Add New", this);
    refreshList = new QPushButton("Refresh", this);
    remove = new QPushButton("Remove", this);

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
    hbox->addWidget(remove);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    setLayout(mainLayout);
    setupSignal();
}

void UserWindow::setupSignal()
{
    connect(newUserWidget->getSaveButton(), &QPushButton::clicked, this, &UserWindow::populateData);

    connect(refreshList, &QPushButton::clicked, this, &UserWindow::populateData);

    connect(usersList, &QTableView::clicked, [this](const QModelIndex &index){
        if(index.row() == -1)
            return;

        auto id = modelData->data(modelData->index(index.row(), 0)).toInt();
        QStringList list = (type == USER_TYPE::MEDIA_HOUSE)?io->sql->getMediaHouseRow(id): io->sql->getClientRow(id);
        if(list.isEmpty())
            return;
        newUserWidget->setValues(list);

        populateData();
    });

    connect(remove, &QPushButton::clicked, [this]{
        auto cindex = usersList->currentIndex();
        auto id = modelData->data(modelData->index(cindex.row(), 0)).toInt();
        auto name = modelData->data(modelData->index(cindex.row(), 1)).toString();
        QMessageBox msgBox(QMessageBox::Warning, tr("Do you want to remove"), tr("Do you want to remove <b>'%0'</b> from %1").arg(name).arg((type == USER_TYPE::MEDIA_HOUSE)?"Media House List":"Clients List"), QMessageBox::Yes | QMessageBox::No, this);
        if(msgBox.exec() == QMessageBox::No)
            return;
        (type == USER_TYPE::CLIENT)?io->sql->removeClientRow(id):io->sql->removeMediHouseaRow(id);

        populateData();
        newUserWidget->clearValues();
        usersList->clearSelection();
    });

//    connect(io->sql->getMediaHouseModel(), &QSqlTableModel::dataChanged, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()){
//        populateData();
//    });
}

void UserWindow::populateData()
{
    (type == USER_TYPE::MEDIA_HOUSE)? io->sql->getMediaHouseModel()->query().exec() : io->sql->getClietModel()->query().exec();
    usersList->resizeColumnsToContents();
    usersList->viewport()->update();
}
