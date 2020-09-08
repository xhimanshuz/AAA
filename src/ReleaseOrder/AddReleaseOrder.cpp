#include "ReleaseOrder/AddReleaseOrder.h"

AddReleaseOrder::AddReleaseOrder(QWidget *parent): QDialog(parent)
{
    io = IOHandler::getInstance();
    render();
}

AddReleaseOrder::AddReleaseOrder(const int roNo, QWidget *parent):QDialog(parent)
{
    io = IOHandler::getInstance();

    render();
    auto strList = io->sql->getROStringList(roNo);
    if(!strList.isEmpty())
        setValues(strList);
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
    mediaHousList->addItems(io->sql->getMediaHouseList());
    jobTypeList = new QComboBox(this);
    jobTypeList->addItems(io->sql->getJobTypeList());
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
    clientList->addItems(io->sql->getClientList());
    caption = new QLineEdit(this);
    dateOfPublication = new QLineEdit(this);
    totalSizeDuration = new QLineEdit(this);
    remarks = new QTextEdit(this);
    hsnCode = new QLineEdit(this);
    amount = new QLineEdit(this);
    netAmount = new QLineEdit(this);
    CGST = new QComboBox(this);
    CGST->addItems(gstList);
    cgstAmount = new QLineEdit(this);
    SGST = new QComboBox(this);
    SGST->addItems(gstList);
    sgstAmount = new QLineEdit(this);
    IGST = new QComboBox(this);
    IGST->addItems(gstList);
    igstAmount = new QLineEdit(this);
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
    hbox->addWidget(cgstAmount);
    form->addRow("CGST", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(SGST);
    hbox->addWidget(sgstAmount);
    form->addRow("SGST", hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(IGST);
    hbox->addWidget(igstAmount);
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
        auto sl = toStringList();
        auto size = sl.size();
        io->sql->insertRoData(sl);
        this->close();
    });
}

QStringList AddReleaseOrder::toStringList()
{
    return {
                        QString::number(code),
                        roNo->text(),
                        date->text(),
                        QString::number(io->sql->getMediaHouseCode(mediaHousList->currentText())),
                        mediaHousList->currentText(),
                        QString::number(io->sql->getClientCode(clientList->currentText())),
                        clientList->currentText(),
                        QString::number(io->sql->getJobTypeCode(jobTypeList->currentText())),
                        jobTypeList->currentText(),
                        caption->text(),
                        editionCentre->text(),
                        dateOfPublication->text(),
                        sizeDuration->text(),
                        totalSizeDuration->text(),
                        guarantedPosition->text(),
                        premium->text(),
                        premiumRemark->toPlainText().toUtf8(),
                        rate->text(),
                        rateRemark->toPlainText(),
                        amount->text(),
                        netAmount->text(),
                        remarks->toPlainText(),
                        "Bill Amount",
                        "Invoice No.",
                        "Payment",
                        "Receipt No.",
                        "Receipt Amount",
                        "MediaBill Amount",
                        CGST->currentText(),
                        cgstAmount->text(),
                        SGST->currentText(),
                        sgstAmount->text(),
                        IGST->currentText(),
                        igstAmount->text(),
                        roAmount->text(),
                        hsnCode->text()
    };
}

void AddReleaseOrder::setValues(const QStringList detailList)
{
    code = detailList.at(0).toInt();
    roNo->setText(detailList.at(1));
    roNo->setDisabled(true);
    date->setDate(QDate()); // 10
    mediaHousList->setCurrentText(detailList.at(4));
    clientList->setCurrentText(detailList.at(6));
    jobTypeList->setCurrentText(detailList.at(8));
    caption->setText(detailList.at(9));
    editionCentre->setText(detailList.at(10));
    dateOfPublication->setText(detailList.at(11));
    sizeDuration->setText(detailList.at(12));
    totalSizeDuration->setText(detailList.at(13));
    guarantedPosition->setText(detailList.at(14));
    premium->setText(detailList.at(15));
    premiumRemark->setText(detailList.at(16));
    rate->setText(detailList.at(17));
    rateRemark->setText(detailList.at(18));
    amount->setText(detailList.at(19));
    netAmount->setText(detailList.at(20));
    remarks->setText(detailList.at(21));
    CGST->setCurrentText(detailList.at(28));
    cgstAmount->setText(detailList.at(29));
    SGST->setCurrentText(detailList.at(30));
    sgstAmount->setText(detailList.at(31));
    IGST->setCurrentText(detailList.at(32));
    igstAmount->setText(detailList.at(33));
    roAmount->setText(detailList.at(34));
    hsnCode->setText(detailList.at(35));
}
