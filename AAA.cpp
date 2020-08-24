#include "AAA.h"

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
    roInvNo = new QComboBox;
    mediaHouse = new QComboBox;

    client = new QComboBox;
    jobType = new QComboBox;
    go = new QPushButton("Go");

    roTable = new QTableView;
    roDataModel = new QStandardItemModel;
    roTable->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
    roTable->setModel(roDataModel);
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
    hbox->addWidget(roInvNo, 1);
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
        ReceiptDetail receipt(this);
        receipt.exec();

        updateRender();
    });

    connect(mediaBill, &QAction::triggered, [this]{
        MediaBill mediabill(this);
        mediabill.exec();

        updateRender();
    });

    connect(payment, &QAction::triggered, [this]{
        PaymentDetail payment(this);
        payment.exec();

        updateRender();
    });



    connect(newRO, &QPushButton::clicked, [this]{
        AddReleaseOrder addRO;
        addRO.exec();

        updateRender();
    });

    connect(roTable, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        auto roNo = roDataModel->item(index.row(), 0)->text().toInt();
            AddReleaseOrder ro(roNo, this);
            ro.exec();
    });

    connect(generateBill, &QAction::triggered, [this](){
        GenerateBillWindow gbw;;
        gbw.exec();
    });
}

void AAA::updateRender()
{
    roInvNo->clear();
    mediaHouse->clear();
    jobType->clear();

    roInvNo->addItems(io->dataEngine->roStringList());
    mediaHouse->addItems(io->dataEngine->mediaHouseStringList());
    jobType->addItems(io->dataEngine->jobTypeStringList());

    populateData();
}

void AAA::populateData()
{
    roDataModel->clear();
    roDataModel->setHorizontalHeaderLabels(QStringList()<<"RO No."<<"Media House"<< "Parties"<<"Job Type"<<"Caption"<<"HSN"<< "Net Amt"<<
                                           "Ro Amt"<< "Media Bill"<< "Payment"<<"Bill No"<< "Bill Amt"<<"Rcpt No."
                                           "Receipt");
    for(auto ro: *io->dataEngine->ROMap)
    {
        roDataModel->appendRow(QList<QStandardItem*>() << new QStandardItem(ro.at(RO::RONO))
                               << new QStandardItem(ro.at(RO::DATE)) << new QStandardItem(ro.at(RO::MEDIAHOUSELIST)) << new QStandardItem(ro.at(RO::CLIENTLIST))
                               << new QStandardItem(ro.at(RO::JOBTYPELIST)) << new QStandardItem(ro.at(RO::CAPTION)) << new QStandardItem(ro.at(RO::HSNCODE))
                               << new QStandardItem(ro.at(RO::NETAMOUNT)) << new QStandardItem(ro.at(RO::ROAMOUNT)) << new QStandardItem("MEDIA BILL")
                               << new QStandardItem("PAYMENT") << new QStandardItem("BILL NO") << new QStandardItem("BILL AMT")
                               << new QStandardItem("RCPT"));

    }

}
