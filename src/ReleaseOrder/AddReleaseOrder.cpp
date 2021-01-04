#include "ReleaseOrder/AddReleaseOrder.h"
#include "pdftroninterface.h"


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
    QStringList gstList = *io->sql->getGstPerc();
    mainLayout = new QVBoxLayout;
    auto mainHBox = new QHBoxLayout;

    mainLayout->addWidget(new QLabel("<h1>Add New Release Order</h1>"));

    roNo = new QLineEdit(this);
    roNo->setText(QString::number(io->sql->getNewRoNumber()));
    roNo->setReadOnly(true);
    mediaHousList = new QComboBox(this);
    mediaHousList->addItems(io->sql->getMediaHouseList());
    jobTypeList = new QComboBox(this);
    jobTypeList->addItems(io->sql->getJobTypeList());
    editionCentre = new QLineEdit(this);
    sizeDuration = new QLineEdit(this);
//    sizeDuration->setValidator(new QRegExpValidator(QRegExp("\\d*{4}")));
    guarantedPosition = new QLineEdit(this);
    premium = new QLineEdit(this);
    premiumRemark = new QTextEdit(this);
    premiumRemark->setTabChangesFocus(true);
//    premiumRemark->setSizePolicy(QSizePolicy::QSizePolicy::Fixed,QSizePolicy::Preferred);
    rate = new QLineEdit(this);
    rateRemark = new QTextEdit(this);
    rateRemark->setTabChangesFocus(true);
    date = new QDateEdit(QDate::currentDate(), this);
    date->setDisplayFormat("dd/MM/yyyy");
    date->setCalendarPopup(true);
    clientList = new QComboBox(this);
    clientList->addItems(io->sql->getClientList());
    caption = new QLineEdit(this);
    dateOfPublication = new QLineEdit(this);
    totalSizeDuration = new QLineEdit(this);
    remarks = new QTextEdit(this);
    remarks->setTabChangesFocus(true);
    hsnCode = new QLineEdit(this);
    amount = new QLineEdit(this);
    discountPer = new QDoubleSpinBox;
    discountPer->setRange(0,100);
    discount = new QLineEdit("0.00");
    discount->setPlaceholderText("Discount Amount");
    netAmount = new QLineEdit(this);
    CGST = new QComboBox(this);
    CGST->addItems(gstList);
    cgstAmount = new QLineEdit(this);
    cgstAmount->setReadOnly(true);
    SGST = new QComboBox(this);
    SGST->addItems(gstList);
    sgstAmount = new QLineEdit(this);
    sgstAmount->setReadOnly(true);
    IGST = new QComboBox(this);
    IGST->addItems(gstList);
    igstAmount = new QLineEdit(this);
    igstAmount->setReadOnly(true);
    roAmount = new QLineEdit(this);
    save = new QPushButton("Save", this);
    clear = new QPushButton("Clear", this);
    printButton = new QPushButton("Print", this);
    calculateRoAmount = new QPushButton("Calculate");

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

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Discount"));
    hbox->addWidget(discountPer);
    hbox->addWidget(discount);
    hbox->addWidget(netAmount);
    form->addRow("Net Amount", hbox);

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

    hbox = new QHBoxLayout;
    hbox->addWidget(roAmount);
    hbox->addWidget(calculateRoAmount);
    form->addRow("RO Amount", hbox);
    mainHBox->addLayout(form);

    mainLayout->addLayout(mainHBox);

    hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(clear);
    hbox->addWidget(printButton);
    hbox->addStretch();
    mainLayout->addLayout(hbox);
    mainLayout->addStretch();


    setLayout(mainLayout);
    setupSignal();
    setValidator();
}

void AddReleaseOrder::setupSignal()
{
    connect(save, &QPushButton::clicked, [this]{
        auto sl = toStringList();
        io->sql->insertRoData(sl);
        this->close();
    });

    connect(printButton, &QPushButton::clicked, [this]{
        for(auto mpList: io->sql->getMediaPaymentStringListByRono(roNo->text().toInt()))
            PDFTronInterface::get()->printRO(toStringList(), mpList);
    });

    connect(CGST, &QComboBox::currentTextChanged, [this](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
            tax = QString::number(0);
        else
            IGST->setCurrentIndex(0);
        auto cgstAmt = (tax.toDouble()/100)*netAmount->text().toDouble();
        cgstAmount->setText(QString::number(cgstAmt));
        roAmount->setText(QString::number(netAmount->text().toDouble()+cgstAmt + sgstAmount->text().toDouble()));
    });

    connect(SGST, &QComboBox::currentTextChanged, [this](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
            tax = QString::number(0);
        else
            IGST->setCurrentIndex(0);
        auto sgstAmt = (tax.toDouble()/100)*netAmount->text().toDouble();
        sgstAmount->setText(QString::number(sgstAmt));
        roAmount->setText(QString::number(netAmount->text().toDouble() + sgstAmt + cgstAmount->text().toDouble()));
    });

    connect(IGST, &QComboBox::currentTextChanged, [this](const QString taxNumber){
        QString tax = taxNumber;
        if(tax.isEmpty())
        {
            igstAmount->setText(QString::number(0));
            return;
        }

        SGST->setCurrentIndex(0);
        CGST->setCurrentIndex(0);
        auto igstAmt = (tax.toDouble()/100)*netAmount->text().toDouble();
        igstAmount->setText(QString::number(igstAmt));
        roAmount->setText(QString::number(netAmount->text().toDouble()+igstAmt));
    });

    connect(discountPer, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double perc){
        if(discount->hasFocus() || netAmount->hasFocus())
            return;
        auto disAmount = (perc/100)*amount->text().toDouble();
        discount->setText(QString::number(disAmount));
        if(!netAmount->hasFocus())
            netAmount->setText(QString::number(amount->text().toDouble() - disAmount));
    });

    connect(discount, &QLineEdit::textChanged, [=](const QString value){
        if(discountPer->hasFocus())
            return;
        auto disPer = (value.toDouble()/amount->text().toDouble()) * 100;
        discountPer->setValue(disPer);
        if(!netAmount->hasFocus())
            netAmount->setText(QString::number(amount->text().toDouble()-value.toDouble()));
    });

    connect(amount, &QLineEdit::textChanged, [=](const QString){
//        discount->setText(discount->text());
        discountPer->valueChanged(discountPer->value());
        discount->textChanged(discount->text());
    });

    connect(netAmount, &QLineEdit::textChanged, [=](const QString value){
        if(discount->hasFocus() || discountPer->hasFocus())
            return;
        auto disA = amount->text().toDouble() - value.toDouble();
        discount->setText(QString::number(disA));
    });

    connect(calculateRoAmount, &QPushButton::clicked, [=]{
        netAmount->textChanged(netAmount->text());
        netAmount->setText(netAmount->text());
        CGST->currentTextChanged(CGST->currentText());
        SGST->currentTextChanged(SGST->currentText());
        IGST->currentTextChanged(IGST->currentText());
    });

}

QStringList AddReleaseOrder::toStringList()
{
    return {
                        QString::number(0),
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
                        billAmount, //
                        invoiceNo,
                        payment,
                        recptNo,
                        receiptAmount,
                        mediaBillAmount,
                        CGST->currentText(),
                        cgstAmount->text(),
                        SGST->currentText(),
                        sgstAmount->text(),
                        IGST->currentText(),
                        igstAmount->text(),
                        roAmount->text(),
                        hsnCode->text(),
                        QString::number(discountPer->value())
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
    amount->setText(QString::number(detailList.at(19).toDouble()));
    netAmount->setText(QString::number(detailList.at(20).toDouble()));
    remarks->setText(detailList.at(21));

    billAmount = detailList.at(22);
    invoiceNo = detailList.at(23);
    payment = detailList.at(24);
    recptNo = detailList.at(25);
    receiptAmount = detailList.at(26);
    mediaBillAmount = detailList.at(27);

    CGST->setCurrentText(detailList.at(28));
//    cgstAmount->setText(detailList.at(29));
    SGST->setCurrentText(detailList.at(30));
//    sgstAmount->setText(detailList.at(31));
    IGST->setCurrentText(detailList.at(32));
//    igstAmount->setText(detailList.at(33));
//    roAmount->setText(QString::number(detailList.at(34).toDouble()));
    hsnCode->setText(detailList.at(35));
//    discountPer->setValue(detailList.at(36).toDouble()); // It should be disable because old values don't have discount feature
}

void AddReleaseOrder::setValidator()
{
//    roNo->setValidator(new QRegExpValidator(QRegExp("\\d+")));
//    caption->setValidator(new QRegExpValidator(QRegExp("\\d+")));
//    editionCentre->setValidator(new QRegExpValidator(QRegExp("\\w+")));
//    sizeDuration->setValidator(new QRegExpValidator(QRegExp()));
//    totalSizeDuration->setValidator(new QRegExpValidator(QRegExp()));
//    guarantedPosition->setValidator(new QRegExpValidator(QRegExp()));
    premium->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    rate->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    amount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    netAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    cgstAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    sgstAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    igstAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    roAmount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
    hsnCode->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    discount->setValidator(new QRegExpValidator(QRegExp("\\d+\\.?\\d+")));
}
