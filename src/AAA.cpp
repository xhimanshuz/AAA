#include "AAA.h"
#include "Config.h"
#include "pdftroninterface.h"
#include <QSqlDatabase>

AAA::AAA(QWidget *parent) : QMainWindow(parent)
{

    io = IOHandler::getInstance();
    appConfigure();


    render();
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void AAA::render()
{
    exit = new QAction("Exit");
    menuBar()->addMenu("File")->addAction(exit);
    menuBar()->setStyleSheet("color: white; background-color: #212121");

    toolBar = new QToolBar("ToolBar");
    newJobType = new QAction("New Job Type");
    newMediaHouse = new QAction("New Media House");
    newClient = new QAction("New Client");
    mediaPaymentButton = new QAction("Media Payment");
    paymentReceipt = new QAction("Payment Receipt");
    invoiceButton = new QAction("Invoice");
    mediaBill = new QAction("Media Bill");

    toolBar->addActions(QList<QAction*>()<< newJobType << newMediaHouse << newClient << mediaPaymentButton << invoiceButton  << paymentReceipt  << mediaBill);

    searchDateFrom = new QDateEdit(QDate::currentDate(), this);
    searchDateFrom->setDisplayFormat("yyyy-MM-dd");
    searchDateFrom->setCalendarPopup(true);
    searchDateTo = new QDateEdit(QDate::currentDate(), this);
    searchDateTo->setDisplayFormat("yyyy-MM-dd");
    searchDateTo->setCalendarPopup(true);
    dateSearchButton = new QToolButton;
    dateSearchButton->setText("⏲");
    dateClearButton = new QToolButton;
    dateClearButton->setText("⌫");
    roNo = new QComboBox;
    roNo->setEditable(true);
    mediaHouse = new QComboBox;
    mediaHouse->setEditable(true);
//    mediaHouse->completer()->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);

    client = new QComboBox;
    client->setEditable(true);
//    client->completer()->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);
    jobType = new QComboBox;

    jobType->setEditable(true);
    go = new QPushButton("Go");

    roTable = new QTableView;
    roDataModel = io->sql->getRoModel();
    roTable->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
    roTable->setEditTriggers(QTableView::NoEditTriggers);
    roTable->setSelectionMode(QTableView::SelectionMode::SingleSelection);
    roTable->setModel(roDataModel);
    for(auto i: {0, 3,4, 5,7,9,10,11,12,13,14,15,16,17,18,21,28,29,30,31,32,33,35,36})
        roTable->hideColumn(i);
    roTable->verticalHeader()->setVisible(false);
    roTable->horizontalHeader()->setStretchLastSection(true);

    populateData();

    newRO = new QPushButton("New RO");
    printList = new QPushButton("Print List");
    setting = new QPushButton("Setting");

    mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(toolBar);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Date From"));
    hbox->addWidget(searchDateFrom, 1);
    hbox->addWidget(new QLabel("To"));
    hbox->addWidget(searchDateTo, 1);
    hbox->addWidget(dateSearchButton);
    hbox->addWidget(dateClearButton);
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

//    QWidget *statusBarWidget = new QWidget;
//    hbox = new QHBoxLayout;
//    hbox->addWidget(newRO);
//    hbox->addWidget(printList);
//    hbox->addWidget(setting);
//    hbox->addStretch();
//    statusBarWidget->setLayout(hbox);

//    statusBarWidget->setStyleSheet("color: white; background-color: #212121;");
    statusBar()->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    statusBar()->addWidget(newRO);
    statusBar()->addWidget(printList);
    statusBar()->addWidget(setting);
    statusBar()->setStyleSheet("color: white; background-color: #212121");
    statusBar()->showMessage("Application Started", 10000);
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

    connect(paymentReceipt, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        PaymentReceiptDetail receipt(this, roNo);
        receipt.exec();

        updateRender();
    });

    connect(mediaBill, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        MediaBill mediabill(this, roNo);
        mediabill.exec();

        updateRender();
    });

    connect(mediaPaymentButton, &QAction::triggered, [this]{
        auto roNo = getRoNumber();
        MediaPaymentDetail payment(this, roNo);
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

    connect(invoiceButton, &QAction::triggered, [this](){
        auto index = roTable->currentIndex();
        auto client = roDataModel->data(roDataModel->index(index.row(), 6)).toString();
        auto invno = roDataModel->data(roDataModel->index(index.row(), 23)).toString().split(",");
        auto ro = getRoNumber();
        InvoiceWindow gbw(client, invno, ro, this);
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

    connect(dateSearchButton, &QToolButton::clicked, [=]{

        auto fromDate = searchDateFrom->text();
        auto toDate = searchDateTo->text();
        io->sql->getRoModel()->setFilter(QString("date >= '%0' and date <= '%1'").arg(fromDate).arg(toDate));
        populateData();
    });

    connect(dateClearButton, &QToolButton::clicked, [=]{
        auto date = QDate::currentDate().toString("yyyy-MM-dd");
        io->sql->getRoModel()->setFilter(QString("date < '%0'").arg(date));
        populateData();
    });

    connect(setting, &QPushButton::clicked, [this]{
        ConfigUI ui(io, this);
        ui.exec();
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
    roTable->sortByColumn(1, Qt::SortOrder::DescendingOrder);
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

void AAA::appConfigure()
{
    config = Configure::get();
    config->setConfigFromList(io->sql->getConfigList());
    PDFTronInterface::get()->setPdfApplication(config->getPdfApplication());
//    Config::get()->setConfigFromList(io->sql->getConfig());

}

