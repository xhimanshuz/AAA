#include "AAA.h"
#include <QSqlDatabase>

AAA::AAA(QWidget *parent) : QMainWindow(parent)
{

    io = IOHandler::getInstance();

    render();
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void AAA::render()
{
    exit = new QAction("Exit");
    menuBar()->addMenu("File")->addAction(exit);

    toolBar = new QToolBar("ToolBar");
    newJobType = new QAction("New Job Type");
    newMediaHouse = new QAction("New Media House");
    newClient = new QAction("New Client");
    payment = new QAction("Payment");
    receipt = new QAction("Receipt");
    generateBill = new QAction("Generate Bill");
    mediaBill = new QAction("Media Bill");

    toolBar->addActions(QList<QAction*>()<< newJobType << newMediaHouse << newClient << payment << receipt << generateBill << mediaBill);

    searchDateFrom = new QDateEdit;
    searchDateTo = new QDateEdit;
    roNo = new QComboBox;
    roNo->setEditable(true);
    mediaHouse = new QComboBox;
    mediaHouse->setEditable(true);
//    mediaHouse->completer()->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);

    client = new QComboBox;
    client->setEditable(true);
//    client->completer()->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
    jobType = new QComboBox;
//    jobType->completer()->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
    jobType->setEditable(true);
    go = new QPushButton("Go");

    roTable = new QTableView;
    roDataModel = io->sql->getRoModel();
    roTable->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
    roTable->setEditTriggers(QTableView::NoEditTriggers);
    roTable->setModel(roDataModel);
    roTable->hideColumn(0);
    populateData();

    newRO = new QPushButton("New RO");
    printList = new QPushButton("Print List");

    mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(toolBar);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Search Date From"));
    hbox->addWidget(searchDateFrom, 1);
    hbox->addWidget(new QLabel("To"));
    hbox->addWidget(searchDateTo, 1);
    hbox->addWidget(new QLabel("RO/Inv No."));
    hbox->addWidget(roNo, 1);
    hbox->addWidget(new QLabel("Media House"));
    hbox->addWidget(mediaHouse, 1);
    hbox->addWidget(new QLabel("Client"));
    hbox->addWidget(client,1);
    hbox->addWidget(new QLabel("Job Type"));
    hbox->addWidget(jobType);
    hbox->addWidget(go);
    mainLayout->addLayout(hbox);

    mainLayout->addWidget(roTable);

    hbox = new QHBoxLayout;
    hbox->addWidget(newRO);
    hbox->addWidget(printList);
    hbox->addStretch();
    mainLayout->addLayout(hbox);


    setupSignals();
    updateRender();
}

void AAA::setupSignals()
{

    connect(newJobType, &QAction::triggered, [this]{
        JobType jobType;
        jobType.exec();

        updateRender();
    });

    connect(newMediaHouse, &QAction::triggered, [this]{
        UserWindow mediaHouse(USER_TYPE::MEDIA_HOUSE, this);
        mediaHouse.exec();

        updateRender();
    });

    connect(newClient, &QAction::triggered, [this]{
        UserWindow client(USER_TYPE::CLIENT, this);
        client.exec();

        updateRender();
    });

    connect(receipt, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        ReceiptDetail receipt(this, roNo);
        receipt.exec();

        updateRender();
    });

    connect(mediaBill, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        MediaBill mediabill(this, roNo);
        mediabill.exec();

        updateRender();
    });

    connect(payment, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        PaymentDetail payment(this, roNo);
        payment.exec();

        updateRender();
    });


    connect(newRO, &QPushButton::clicked, [this]{
        AddReleaseOrder addRO;
        addRO.exec();

        updateRender();
    });

    connect(roTable, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        auto roNo = getRoNumber(index);
            AddReleaseOrder ro(roNo, this);
            ro.exec();
    });

    connect(generateBill, &QAction::triggered, [this](){
        auto index = roTable->currentIndex();
        auto client = roDataModel->data(roDataModel->index(index.row(), 6)).toString();
        auto invno = roDataModel->data(roDataModel->index(index.row(), 23)).toInt();
        auto ro = getRoNumber();
        GenerateBillWindow gbw(client, invno, ro, this);
        gbw.exec();
        populateData();
    });

    connect(mediaHouse, &QComboBox::currentTextChanged, [this](const QString &mediaHouse){
        if(mediaHouse.isEmpty())
        {
            populateData();
            return ;
        }
        io->sql->getRoModel()->setFilter(QString("mhname = '%0'").arg(mediaHouse));
        populateData();
    });

    connect(client, &QComboBox::currentTextChanged, [this](const QString &client){
        if(client.isEmpty())
        {
            io->sql->getRoModel()->setFilter("");
            populateData();
            return ;
        }

        io->sql->getRoModel()->setFilter(QString("pname = '%0'").arg(client));
        populateData();
    });

    connect(jobType, &QComboBox::currentTextChanged, [this](const QString &jobType){
        if(jobType.isEmpty())
        {
            io->sql->getRoModel()->setFilter("");
            populateData();
            return ;
        }

        io->sql->getRoModel()->setFilter(QString("jobtypename = '%0'").arg(jobType));
        populateData();
    });

    connect(roNo, &QComboBox::currentTextChanged, [this](const QString &ronumber){
        if(ronumber.isEmpty())
        {
            io->sql->getRoModel()->setFilter("");
            populateData();
            return ;
        }

        io->sql->getRoModel()->setFilter(QString("number = %0").arg(ronumber.toInt()));
        populateData();
    });
}

void AAA::updateRender()
{
    roNo->clear();
    mediaHouse->clear();
    jobType->clear();
    client->clear();

    roNo->addItems(io->sql->getRoList());
    mediaHouse->addItems(io->sql->getMediaHouseList());
    jobType->addItems(io->sql->getJobTypeList());
    client->addItems(io->sql->getClientList());
    populateData();
}

void AAA::populateData()
{
    io->sql->getRoModel()->query().exec();
//    roTable->resizeRowsToContents();
    roTable->resizeColumnsToContents();
    roTable->setSortingEnabled(1);
    roTable->viewport()->update();
}

int AAA::getRoNumber(const QModelIndex &index)
{
    return roDataModel->data(roDataModel->index(index.row(), 1)).toInt();
}

int AAA::getRoNumber()
{
    auto index = roTable->currentIndex();
    return roDataModel->data(roDataModel->index(index.row(), 1)).toInt();
}

