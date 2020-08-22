#include "ReleaseOrder/AddReleaseOrder.h"

AddReleaseOrder::AddReleaseOrder(QWidget *parent): QWidget(parent)
{
    io = IOHandler::getInstance();
    render();
}

AddReleaseOrder::AddReleaseOrder(const QStringList &detailList, QWidget *parent)
{

}

AddReleaseOrder::~AddReleaseOrder()
{

}

void AddReleaseOrder::render()
{
    QStringList gstList = {"2.5%", "6%","9%", "14%", "5%", "12%", "18%", "28%"};
    mainLayout = new QVBoxLayout;
    auto mainHBox = new QHBoxLayout;

    mainLayout->addWidget(new QLabel("<h1>Add New Release Order</h1>"));

    roNo = new QLineEdit(this);
    mediaHousList = new QComboBox(this);
    mediaHousList->addItems(io->dataEngine->mediaHouseStringList());
    jobTypeList = new QComboBox(this);
    jobTypeList->addItems(io->dataEngine->jobTypeStringList());
    editionCentre = new QLineEdit(this);
    sizeDuration = new QLineEdit(this);
    guarantedPosition = new QLineEdit(this);
    premium = new QLineEdit(this);
    premiumRemark = new QTextEdit(this);
//    premiumRemark->setSizePolicy(QSizePolicy::QSizePolicy::Fixed,QSizePolicy::Preferred);
    rate = new QLineEdit(this);
    rateRemark = new QTextEdit(this);
    date = new QDateEdit(QDate::currentDate(), this);
    clientList = new QComboBox(this);
    clientList->addItems(io->dataEngine->clientStringList());
    caption = new QLineEdit(this);
    dateOfPublication = new QLineEdit(this);
    totalSizeDuration = new QLineEdit(this);
    remarks = new QTextEdit(this);
    hsnCode = new QLineEdit(this);
    amount = new QLineEdit(this);
    netAmount = new QLineEdit(this);
    CGST = new QComboBox(this);
    CGST->addItems(gstList);
    cgstRemark = new QLineEdit(this);
    SGST = new QComboBox(this);
    SGST->addItems(gstList);
    sgstRemark = new QLineEdit(this);
    IGST = new QComboBox(this);
    IGST->addItems(gstList);
    igstRemark = new QLineEdit(this);
    roAmount = new QLineEdit(this);
    save = new QPushButton("Save", this);
    clear = new QPushButton("Clear", this);

    QFormLayout *form = new QFormLayout;
    form->addRow("RO No", roNo);
    form->addRow("Media House", mediaHousList);
    form->addRow("Job Type", jobTypeList);
    form->addRow("Edition/Centre", editionCentre);
    form->addRow("Size/Duration", sizeDuration);
    form->addRow("Guaranted Position", guarantedPosition);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(premium, 1, Qt::AlignmentFlag::AlignTop);
    hbox->addWidget(premiumRemark,1);
    form->addRow("Premium", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(rate, 1, Qt::AlignmentFlag::AlignTop);
    hbox->addWidget(rateRemark, 1);
    form->addRow("Rate", hbox);

    mainHBox->addLayout(form);

    form = new QFormLayout;
    form->addRow("Date", date);
    form->addRow("Clients", clientList);
    form->addRow("Caption", caption);
    form->addRow("Date of Publication/Telecast", dateOfPublication);
    form->addRow("Total Size/Duration", totalSizeDuration);
    form->addRow("Remark", remarks);
    form->addRow("HSN Code", hsnCode);
    form->addRow("Amount", amount);
    form->addRow("Net Amount", netAmount);

    hbox = new QHBoxLayout;
    hbox->addWidget(CGST);
    hbox->addWidget(cgstRemark);
    form->addRow("CGST", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(SGST);
    hbox->addWidget(sgstRemark);
    form->addRow("SGST", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(IGST);
    hbox->addWidget(igstRemark);
    form->addRow("IGST", hbox);

    form->addRow("RO Amount", roAmount);
    mainHBox->addLayout(form);

    mainLayout->addLayout(mainHBox);

    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addStretch();
    mainLayout->addLayout(hbox);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setupSignal();
}

void AddReleaseOrder::setupSignal()
{
    connect(save, &QPushButton::clicked, [this]{
        io->dataEngine->insertROData(toStringList());
    });
}

QStringList AddReleaseOrder::toStringList()
{
    return {roNo->text(), mediaHousList->currentText(), jobTypeList->currentText(), editionCentre->text(),
                sizeDuration->text(), guarantedPosition->text(), premium->text(), premiumRemark->toPlainText().toUtf8(),
                rate->text(), rateRemark->toPlainText(), date->text(), clientList->currentText(), dateOfPublication->text(),
                totalSizeDuration->text(), totalSizeDuration->text(), remarks->toPlainText(), hsnCode->text(), amount->text(),
                netAmount->text(), CGST->currentText(), cgstRemark->text(), SGST->currentText(), sgstRemark->text(),
                IGST->currentText(), igstRemark->text(), roAmount->text()};
}

void AddReleaseOrder::setValues(const QStringList detailList)
{
    roNo->setText(detailList.at(0));
    mediaHousList->setCurrentText(detailList.at(1));
    jobTypeList->setCurrentText(detailList.at(2));
    editionCentre->setText(detailList.at(3));
    sizeDuration->setText(detailList.at(4));
    guarantedPosition->setText(detailList.at(5));
    premium->setText(detailList.at(6));
    premiumRemark->setText(detailList.at(7));
    rate->setText(detailList.at(8));
    rateRemark->setText(detailList.at(9));
    date->setDate(QDate()); // 10
    clientList->setCurrentText(detailList.at(11));
    caption->setText(detailList.at(12));
    dateOfPublication->setText(detailList.at(13));
    totalSizeDuration->setText(detailList.at(14));
    remarks->setText(detailList.at(15));
    hsnCode->setText(detailList.at(16));
    amount->setText(detailList.at(17));
    netAmount->setText(detailList.at(18));
    CGST->setCurrentText(detailList.at(19));
    cgstRemark->setText(detailList.at(20));
    SGST->setCurrentText(detailList.at(21));
    sgstRemark->setText(detailList.at(22));
    IGST->setCurrentText(detailList.at(23));
    igstRemark->setText(detailList.at(24));
    roAmount->setText(detailList.at(25));
}
