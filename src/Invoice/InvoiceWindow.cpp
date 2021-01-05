#include "InvoiceWindow.h"

InvoiceWindow::InvoiceWindow(const QString _client, const QStringList invno, const int rono, QWidget *parent) : QDialog(parent), islastSelectedMedia(false)
{
    this->resize(parent->size().width()*0.8, parent->size().height()*0.8);
    io = IOHandler::getInstance();

    addInvoice = new AddInvoice(this, invno.at(0).toInt(), rono);
    render();
    setLayout(mainLayout);

//    if(!_client.isEmpty())
//        filterWithClient(_client);
    if(invno.at(0) != "")
        addInvoice->setValueFromNumbers(invno);
//        generateBill->setValue(io->sql->getInvoiceList(invno.at(0).toInt()));
    else if(rono > 0)
        addInvoice->setValueFromRO(io->sql->getROStringList(rono));
}

InvoiceWindow::~InvoiceWindow()
{

}

void InvoiceWindow::render()
{
    mainLayout = new QVBoxLayout;

    seachRoNo = new QComboBox;
    seachRoNo->addItems(io->sql->getRoList());
    seachRoNo->setEditable(true);
    seachRoNo->setInsertPolicy(QComboBox::InsertPolicy::NoInsert);
    mediaHouse = new QComboBox;
    mediaHouse->addItems(io->sql->getMediaHouseList());
    status = new QComboBox;
    status->addItems(QStringList()<< "Pending"<< "All");
    freshList = new QPushButton("Fresh List");

    billListView = new QTableView;
    billListView->setSelectionBehavior(QTableView::SelectRows);
    billListView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
    billListView->setEditTriggers(QTableView::NoEditTriggers);
    billListView->horizontalHeader()->setStretchLastSection(true);
    billDataModel = io->sql->getInvoiceModel();
    billListView->setModel(billDataModel);
    populateData();


    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Search RO No"));
    hbox->addWidget(seachRoNo);
    hbox->addWidget(new QLabel("Media House"));
    hbox->addWidget(mediaHouse);
    hbox->addWidget(status);
    hbox->addWidget(freshList);
    hbox->addStretch();
    mainLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(billListView, 2);
    hbox->addWidget(addInvoice, 1);
    mainLayout->addLayout(hbox);

    setupSignals();
}

void InvoiceWindow::setupSignals()
{
    connect(seachRoNo, &QComboBox::currentTextChanged, this, &InvoiceWindow::filterWithRO);
    connect(addInvoice, &AddInvoice::saveClicked, this, &InvoiceWindow::populateData);
    connect(addInvoice, &AddInvoice::currentClientChanged, this, &InvoiceWindow::filterWithClient);

    connect(billListView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        auto invoice = billDataModel->data(billDataModel->index(index.row(), 1)).toInt();
        auto ro = billDataModel->data(billDataModel->index(index.row(), 0)).toInt();

        if(invoice > 0)
            addInvoice->setValue(io->sql->getInvoiceList(invoice));
        else
            addInvoice->setValueFromRO(io->sql->getROStringList(ro));
    });

    connect(status, &QComboBox::currentTextChanged, this, &InvoiceWindow::filterWithStatus);
    connect(mediaHouse, &QComboBox::currentTextChanged, [this](const QString mediaHouse){
        islastSelectedMedia = true;
        filterWithStatus(mediaHouse);
    });

}

void InvoiceWindow::populateData()
{
    billDataModel->query().exec();
    billListView->viewport()->update();
}

void InvoiceWindow::filterWithClient(const QString _client)
{
    addInvoice->getClients()->setCurrentText(_client);
    if(status->currentText() == "Pending")
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE pname = '%0' AND invno = '' ").arg(_client));
    else
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE pname = '%0' AND invno != '' ").arg(_client));

    seachRoNo->setCurrentIndex(0);
    populateData();
}

void InvoiceWindow::filterWithStatus(const QString _status)
{
    seachRoNo->setCurrentIndex(0);
    if(islastSelectedMedia)
    {
        auto mhname = mediaHouse->currentText();
        if(_status == "Pending")
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE mhname = '%0' AND invno = ''").arg(mhname));
        }
        else
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE mhname = '%0'").arg(mhname));
        }
    }
    else
    {
        auto pname = addInvoice->getClients()->currentText();
        if(_status == "Pending")
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE pname = '%0' AND invno = ''").arg(pname));
        }
        else
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE pname = '%0'").arg(pname));
        }
    }
    populateData();
}

void InvoiceWindow::filterWithRO(const QString _ro)
{
    if(_ro.isEmpty())
        return;
//    generateBill->getClients()->setCurrentText(_ro);
    if(status->currentText() == "Pending")
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE number = %0 AND invno = '';").arg(_ro));
    else
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, jobtypename, finalamount, netAmount, payamount FROM ro WHERE number = %0  AND invno != ''").arg(_ro));

    populateData();
}


