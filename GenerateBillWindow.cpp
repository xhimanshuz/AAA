#include "GenerateBillWindow.h"

GenerateBillWindow::GenerateBillWindow(QWidget *parent) : QDialog(parent)
{
    io = IOHandler::getInstance();

    render();
    setLayout(mainLayout);
}

GenerateBillWindow::~GenerateBillWindow()
{

}

void GenerateBillWindow::render()
{
    mainLayout = new QVBoxLayout;

    seachRoNo = new QLineEdit;
    mediaHouse = new QComboBox;
    status = new QComboBox;
    freshList = new QPushButton("Fresh List");

    billListView = new QTableView;
    billDataModel = new QStandardItemModel;
    billListView->setModel(billDataModel);

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
    populateData();
}

void GenerateBillWindow::setupSignals()
{
    connect(billListView, &QTableView::clicked, [this](const QModelIndex &index){
        auto text = index.data(Qt::DisplayRole).toString();
        auto roNo = billDataModel->item(index.row(), 0)->text();
        generateBill->setValue(io->dataEngine->gBillStringList(roNo.toInt()));
    });

    connect(generateBill, &AddGenerateBill::saveClicked, this, &GenerateBillWindow::populateData);
}

void GenerateBillWindow::populateData()
{
    billDataModel->clear();
    billDataModel->setHorizontalHeaderLabels(QStringList()<< "RO No."<< "Bill No."<<"Media House"<< "Parties"<< "Net Amt"<<"HSN");

    for(auto gb: *io->dataEngine->getGBillMap())
    {
        billDataModel->appendRow(QList<QStandardItem*>()<< new QStandardItem(gb.at(0)) << new QStandardItem(gb.at(1)) << new QStandardItem(gb.at(2))
                                 << new QStandardItem(gb.at(3)) << new QStandardItem(gb.at(7)) << new QStandardItem(""));
    }

    mediaHouse->addItems(io->dataEngine->mediaHouseStringList());
}

