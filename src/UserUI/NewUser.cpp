#include "UserUI/NewUser.h"

QPushButton *NewUser::getSaveButton() const
{
    return save;
}

NewUser::NewUser(USER_TYPE _type, QWidget *parent) : QWidget(parent), type(_type)
{
    render();
    this->setLayout(mainLayout);
}

NewUser::NewUser(QString username, QStringList userDetail, USER_TYPE _type, QWidget *parent): QWidget(parent), type(_type)
{
    render();
    setLayout(mainLayout);

    setValues(username, userDetail);
    save->click();
}

NewUser::~NewUser()
{

}

void NewUser::render()
{
    mainLayout = new QVBoxLayout;

    title = new QLabel(tr("<h1>Add New %0</h1>").arg((type == USER_TYPE::CLIENT)?"Client":"Media House"));
    userName = new QLineEdit;

    contactPerson = new QLineEdit;
    phone = new QLineEdit;
    email = new QLineEdit;
    address = new QTextEdit;
    city = new QComboBox;
    city->setEditable(true);
    state = new QComboBox;
    state->setEditable(true);
    gstin = new QLineEdit;
    pinCode = new QLineEdit;
    save = new QPushButton("Save");
    clear = new QPushButton("Clear");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(save);
    hbox->addWidget(clear);

    mainLayout->addWidget(title);
    QFormLayout *form = new QFormLayout;
    form->addRow(tr((type == USER_TYPE::CLIENT)?"Client Name":"Media House"), userName);
    form->addRow("Contact Person", contactPerson);
    form->addRow("Phone ", phone);
    form->addRow("Email ID ", email);
    form->addRow("Address ", address);
    form->addRow("City ", city);
    form->addRow("State ", state);
    form->addRow("GSTIN No. ", gstin);
    form->addRow("Pin Code ", pinCode);
    form->addRow("", hbox);
    mainLayout->addLayout(form);

    signalSetup();
}

void NewUser::setUpStyleSheet()
{
    //    this->setStyleSheet("QLabel {")
}

void NewUser::setValues(const QString client, const QStringList &strList)
{
    userName->setText(client);
    userName->setDisabled(true);
    contactPerson->setText(strList[0]);
    phone->setText(strList[1]);
    email->setText(strList[2]);
    address->setPlainText(strList[3]);
    city->setCurrentText(strList[4]);
    state->setCurrentText(strList[5]);
    gstin->setText(strList[6]);
    pinCode->setText(strList[7]);
}

void NewUser::setValues(const QList<QStandardItem *> &items)
{
    userName->setText(items.at(0)->text());
    userName->setDisabled(true);
    contactPerson->setText(items.at(1)->text());
    phone->setText(items.at(2)->text());
    email->setText(items.at(3)->text());
    address->setPlainText(items.at(4)->text());
    city->setCurrentText(items.at(5)->text());
    state->setCurrentText(items.at(6)->text());
    gstin->setText(items.at(7)->text());
    pinCode->setText(items.at(8)->text());
}

void NewUser::signalSetup()
{
    connect(save, &QPushButton::clicked, [this]{
        if(!isValid())
            return;

        if(type == USER_TYPE::CLIENT)
            IOHandler::getInstance()->dataEngine->insertClientData(userName->text(), toStringList());
        else
            IOHandler::getInstance()->dataEngine->insertMediaHouseData(userName->text(), toStringList());
    });

    connect(clear, &QPushButton::clicked, this, &NewUser::clearValues);

}

void NewUser::clearValues()
{
    if(!userName->isEnabled())
        userName->setEnabled(true);
    userName->clear();
    contactPerson->clear();
    phone->clear();
    email->clear();
    address->clear();
    city->clearEditText();
    state->clearEditText();
    gstin->clear();
    pinCode->clear();
}

bool NewUser::isValid()
{
    if(userName->text().isEmpty() || contactPerson->text().isEmpty() || phone->text().isEmpty() || email->text().isEmpty() || address->toPlainText().isEmpty() || city->currentText().isEmpty()
            || state->currentText().isEmpty() || pinCode->text().isEmpty())
        return false;
    return true;
}

const QStringList NewUser::toStringList()
{
    return {userName->text(), contactPerson->text(), phone->text(), email->text(), address->toPlainText(), city->currentText(), state->currentText(), gstin->text(), pinCode->text()};
}
