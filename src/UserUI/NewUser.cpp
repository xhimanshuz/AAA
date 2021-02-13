#include "UserUI/NewUser.h"

NewUser::NewUser(QString username, QStringList userDetail, USER_TYPE _type, QWidget *parent): QWidget(parent), type(_type), id{0}
{
    render();
    setLayout(mainLayout);

    setValues(userDetail);
    save->click();
}

NewUser::~NewUser()
{

}

QPushButton *NewUser::getSaveButton() const
{
    return save;
}

void NewUser::setValidator()
{
    phone->setInputMask("+\\91-999-999-9999");
    email->setValidator(new QRegExpValidator(QRegExp("(\\w+|\\d+)\\.?(\\w+|\\d+)@\\w+\\D\\.(in|com|co\\.in)")));
    pinCode->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    gstin->setValidator(new QRegExpValidator(QRegExp("\\d{2}[A-Z]{5}\\d{4}[A-Z]{1}[1-9]{1}Z[A-Z]{1}")));
}

NewUser::NewUser(USER_TYPE _type, QWidget *parent) : QWidget(parent), type(_type)
{
    render();
    this->setLayout(mainLayout);
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
    address->setTabChangesFocus(true);
    city = new QComboBox;
    city->setEditable(true);
    state = new QComboBox;
    state->addItems(IOHandler::getInstance()->sql->getStates());
    state->setCurrentText("");
    state->setEditable(true);
    gstin = new QLineEdit;
    pinCode = new QLineEdit;
    stateCode = new QLineEdit;
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
    form->addRow("State ", state);
    form->addRow("City ", city);
    form->addRow("GSTIN No. ", gstin);
    form->addRow("State Code", stateCode);
    form->addRow("Pin Code ", pinCode);
    form->addRow("", hbox);
    mainLayout->addLayout(form);

    signalSetup();
    state->setCurrentText("UTTAR PRADESH");
    setValidator();
}

void NewUser::setUpStyleSheet()
{
    //    this->setStyleSheet("QLabel {")
}

void NewUser::setValues(const QStringList &strList)
{
    id = strList.at(0).toInt();
    userName->setText(strList.at(1));
    userName->setReadOnly(true);
    contactPerson->setText(strList[2]);
    phone->setText(strList[3]);
    email->setText(strList[4]);
    address->setPlainText(strList[5]);
    city->setCurrentText(strList[6]);
    state->setCurrentText(strList[7]);
    gstin->setText(strList[8]);
    stateCode->setText(strList[9]);
    pinCode->setText(strList[10]);
}

void NewUser::setValues(const QList<QVariant> &items)
{
    userName->setText(items.at(0).toString());
    userName->setReadOnly(true);
    contactPerson->setText(items.at(1).toString());
    phone->setText(items.at(2).toString());
    email->setText(items.at(3).toString());
    address->setPlainText(items.at(4).toString());
    city->setCurrentText(items.at(5).toString());
    state->setCurrentText(items.at(6).toString());
    gstin->setText(items.at(7).toString());
    stateCode->setText(items.at(8).toString());
    pinCode->setText(items.at(9).toString());
}

void NewUser::signalSetup()
{
    connect(save, &QPushButton::clicked, [this]{
        if(!isValid())
            return;
        if(type == USER_TYPE::CLIENT)
            IOHandler::getInstance()->sql->insertClientRow(toStringList());
        else
            IOHandler::getInstance()->sql->insertMediaHouseRow(toStringList());

        clearValues();
    });

    connect(clear, &QPushButton::clicked, this, &NewUser::clearValues);
    connect(this, &NewUser::addNewClick, this, &NewUser::clearValues);

    connect(state, &QComboBox::currentTextChanged, [&](const QString &state){
        city->clear();
        city->addItems(IOHandler::getInstance()->sql->getCities(state));
        city->setCurrentText("");
        stateCode->setText(IOHandler::getInstance()->sql->getStateCode(state));
    });

    connect(city, &QComboBox::currentTextChanged, [&](const QString &cityText){
        pinCode->setText(IOHandler::getInstance()->sql->getPinCode(cityText));
    });


}

void NewUser::clearValues()
{
    id = 0;
    if(userName->isReadOnly())
        userName->setReadOnly(false);
    userName->clear();
    contactPerson->clear();
    phone->clear();
    email->clear();
    address->clear();
    city->clearEditText();
    state->clearEditText();
    gstin->clear();
    stateCode->clear();
    pinCode->clear();
}

bool NewUser::isValid()
{
//    if(userName->text().isEmpty() || contactPerson->text().isEmpty() || phone->text().isEmpty() || email->text().isEmpty() || address->toPlainText().isEmpty() || city->currentText().isEmpty()
//            || state->currentText().isEmpty() || pinCode->text().isEmpty())
//        return false;
    if(userName->text().isEmpty())
        return false;
    return true;
}

const QStringList NewUser::toStringList()
{
    return {
                QString::number(id),
                userName->text(),
                contactPerson->text(),
                phone->text(),
                email->text(),
                address->toPlainText(),
                city->currentText(),
                state->currentText(),
                gstin->text(),
                stateCode->text(),
                pinCode->text()
    };
}
