#include "roform.h"

ROForm::ROForm(const QStringList detailList, QWidget *parent) : QWidget(parent)
{

    mainLayout = new QVBoxLayout(this);
    setValue(detailList);
    render();

    setLayout(mainLayout);
}

void ROForm::render()
{
    QPixmap iconPix(":/Data/Images/Data/Images/aaa_logo.png");
    icon.setPixmap(iconPix);
    labelAddress.setText("Anthem Advertising Agency\nB-14, Gandhi Nagar,\nMoradabad - 244001\nTele : 0591-2495626\nCall : 98370 56007\n98977 50235\nEmail : anthem9837056007@gmail.com\n");
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(&icon);
    hbox->addStretch();
    hbox->addWidget(&labelAddress);
    mainLayout->addLayout(hbox);


    hbox = new QHBoxLayout;

    QVBoxLayout *mediaHouseVbox = new QVBoxLayout;
    mediaHouseVbox->addWidget(new QLabel("To,"));
    mediaHouseVbox->addWidget(&mediaHouse);
    mediaHouseVbox->addWidget(&mediaHouseAddress);
    hbox->addLayout(mediaHouseVbox);

    auto rohbox = new QHBoxLayout;
    rohbox->addWidget(new QLabel(QString("R.O. No.: <b>%0</b>").arg(rono.text())));
    rohbox->addWidget(new QLabel(QString("R.O. Date: %0").arg(roDate.text())));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(rohbox);
    vbox->addWidget(&remark);
    hbox->addLayout(vbox);
    mainLayout->addLayout(hbox);

    QVBoxLayout *formLayout = new QVBoxLayout;

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Client"));
    hbox->addWidget(&client);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Caption"));
    hbox->addWidget(&caption);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Edition/Centre"));
    hbox->addWidget(&editionCentre);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Date of Publication/Telecast"));
    hbox->addWidget(&dateOfPublication);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Size/Duration"));
    hbox->addWidget(&sizeDuration);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Total Size/Duration"));
    hbox->addWidget(&totalSizeDuration);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Guaranted Position"));
    hbox->addWidget(&guarantedPosition);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Premium"));
    hbox->addWidget(&premium);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Rate"));
    hbox->addWidget(&rate);
    hbox->addWidget(&rateRemark);
    hbox->addWidget(new QLabel("Amount"));
    hbox->addWidget(&amount);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Net Amount"));
    hbox->addWidget(&netAmount);
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("CGST"));
    hbox->addWidget(new QLabel(QString("@%0 %1").arg(CGST.text()).arg(cgstRate.text())));
    hbox->addWidget(new QLabel("SGST"));
    hbox->addWidget(new QLabel(QString("@%0 %1").arg(SGST.text()).arg(sgstRate.text())));
    hbox->addWidget(new QLabel("IGST"));
    hbox->addWidget(new QLabel(QString("@%0 %1").arg(IGST.text()).arg(igstRate.text())));
    formLayout->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("R.O. Amount"));
    hbox->addWidget(new QLabel(QString("<b>%0</b>").arg(roAmount.text())));
    formLayout->addLayout(hbox);


    hbox = new QHBoxLayout;

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Date"));
    vbox->addWidget(&date);
    hbox->addLayout(vbox);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Amount"));
    vbox->addWidget(&leftAmount);
    hbox->addLayout(vbox);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Mode"));
    vbox->addWidget(&mode);
    hbox->addLayout(vbox);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Cheque No."));
    vbox->addWidget(&chequeNo);
    hbox->addLayout(vbox);

    vbox = new QVBoxLayout;
    vbox->addWidget(new QLabel("Bank Name"));
    vbox->addWidget(&bankName);
    hbox->addLayout(vbox);

    formLayout->addLayout(hbox);
    formLayout->setSpacing(0);
    formLayout->setMargin(0);
    formWidget = new QWidget;
    formWidget->setLayout(formLayout);

    mainLayout->addWidget(formWidget);
    mainLayout->addStretch();
    setStyle();
}

void ROForm::setValue(const QStringList detailList)
{
    rono.setText(detailList.at(1));
    date.setText(detailList.at(2)); // 10
    mediaHouse.setText(detailList.at(4));
    client.setText(detailList.at(6));
//    jobType.setCurrentText(detailList.at(8));
    caption.setText(detailList.at(9));
    editionCentre.setText(detailList.at(10));
    dateOfPublication.setText(detailList.at(11));
    sizeDuration.setText(detailList.at(12));
    totalSizeDuration.setText(detailList.at(13));
    guarantedPosition.setText(detailList.at(14));
    premium.setText(detailList.at(15));
//    premiumRemark.setText(detailList.at(16));
    rate.setText(detailList.at(17));
    rateRemark.setText(detailList.at(18));
    amount.setText(detailList.at(19));
    netAmount.setText(detailList.at(20));
    remark.setText(detailList.at(21));
    CGST.setText(detailList.at(28));
    cgstRate.setText(detailList.at(29));
    SGST.setText(detailList.at(30));
    sgstRate.setText(detailList.at(31));
    IGST.setText(detailList.at(32));
    igstRate.setText(detailList.at(33));
    roAmount.setText(detailList.at(34));

        rono.setTextInteractionFlags(Qt::TextSelectableByMouse);
        client.setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void ROForm::setStyle()
{

    setStyleSheet("background-color: rgb(255, 255, 255); ");
    formWidget->setStyleSheet("border: 0.5px solid black; margin: 0px; padding: 5px;");

}
