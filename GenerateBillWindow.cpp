#include "GenerateBillWindow.h"

GenerateBillWindow::GenerateBillWindow(const QString _client, const int invno, const int rono, QWidget *parent) : QDialog(parent), islastSelectedMedia(false)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);

    if(!_client.isEmpty())
        filterWithClient(_client);
    if(invno > 0)
        generateBill->setValue(io->sql->getGenerateBillList(invno));
    else if(rono > 0)
        generateBill->setValueFromRO(io->sql->getROStringList(rono));
}

GenerateBillWindow::~GenerateBillWindow()
{

}

void GenerateBillWindow::render()
{
    mainLayout = new QVBoxLayout;

    seachRoNo = new QLineEdit;
    mediaHouse = new QComboBox;
    mediaHouse->addItems(io->sql->getMediaHouseList());
    status = new QComboBox;
    status->addItems(QStringList()<< "Pending"<< "All");
    freshList = new QPushButton("Fresh List");

    billListView = new QTableView;
    billListView->setSelectionBehavior(QTableView::SelectRows);
    billListView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
    billListView->setEditTriggers(QTableView::NoEditTriggers);
    billDataModel = io->sql->getGenerateBillModel();
    billListView->setModel(billDataModel);
    populateData();

    generateBill = new AddGenerateBill;

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
    hbox->addWidget(generateBill, 1);
    mainLayout->addLayout(hbox);

    setupSignals();
}

void GenerateBillWindow::setupSignals()
{
    connect(billListView, &QTableView::clicked, [this](const QModelIndex &index){
        auto invoice = billDataModel->data(billDataModel->index(index.row(), 1)).toInt();
        auto ro = billDataModel->data(billDataModel->index(index.row(), 0)).toInt();
        if(invoice > 0)
            generateBill->setValue(io->sql->getGenerateBillList(invoice));
        else
            generateBill->setValueFromRO(io->sql->getROStringList(ro));
    });

    connect(status, &QComboBox::currentTextChanged, this, &GenerateBillWindow::filterWithStatus);
    connect(mediaHouse, &QComboBox::currentTextChanged, [this](const QString mediaHouse){
        islastSelectedMedia = true;
        filterWithStatus(mediaHouse);
    });
    connect(generateBill, &AddGenerateBill::saveClicked, this, &GenerateBillWindow::populateData);
}

void GenerateBillWindow::populateData()
{
    billDataModel->query().exec();
    billListView->viewport()->update();
}

void GenerateBillWindow::filterWithClient(const QString _client)
{
    generateBill->getClients()->setCurrentText(_client);
    if(status->currentText() == "Pending")
    {
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE pname = '%0' AND invno = 0").arg(_client));
    }
    else
    {
        billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE pname = '%0' AND invno != 0").arg(_client));
    }
    populateData();
}

void GenerateBillWindow::filterWithStatus(const QString _status)
{

    if(islastSelectedMedia)
    {
        auto mhname = mediaHouse->currentText();
        if(_status == "Pending")
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE mhname = '%0' AND invno = 0").arg(mhname));
        }
        else
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE mhname = '%0'").arg(mhname));
        }
    }
    else
    {
        auto pname = generateBill->getClients()->currentText();
        if(_status == "Pending")
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE pname = '%0' AND invno = 0").arg(pname));
        }
        else
        {
            billDataModel->setQuery(QString("SELECT number, invno, mhname, pname, netAmount, hsncode FROM ro WHERE pname = '%0'").arg(pname));
        }
    }
    populateData();
}
