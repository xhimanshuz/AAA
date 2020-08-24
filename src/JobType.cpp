#include "JobType.h"
#include <QDebug>

JobType::JobType(QWidget *parent) : QDialog(parent)
{
    io = IOHandler::getInstance();

    render();
    signalSetup();

    setLayout(mainLayout);
}

JobType::~JobType()
{

}

void JobType::render()
{

    mainLayout = new QVBoxLayout;
    jobListView = new QTableView(this);
    jobListView->horizontalHeader()->setStretchLastSection(true);

    dataModel = new QStandardItemModel();
    jobListView->setModel(dataModel);
    updateData();

    mainLayout->addWidget(jobListView);

    addButton = new QPushButton("ADD", this);
    addButton->setCheckable(true);
    removeButton = new QPushButton("REMOVE", this);
    editButton = new QPushButton("EDIT", this);
    editButton->setCheckable(true);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(addButton);
    hbox->addWidget(removeButton);
    hbox->addWidget(editButton);
    mainLayout->addLayout(hbox);

    addLineEdit = new QLineEdit(this);
    saveButton = new QPushButton(tr("INSERT"));
    modifyButton = new QPushButton(tr("Modify"));
    modifyButton->hide();
    addLineEdit->hide();
    saveButton->hide();

    QHBoxLayout *addLayout = new QHBoxLayout;
    addLayout->addWidget(addLineEdit, 2);
    addLayout->addWidget(saveButton, 1);
    addLayout->addWidget(modifyButton, 1);
    mainLayout->addLayout(addLayout);

}

void JobType::signalSetup()
{

    connect(addButton, &QPushButton::toggled, [this](bool toggled){
        if(toggled)
        {
            editButton->setChecked(false);
            addLineEdit->clear();
            addLineEdit->show();
            saveButton->show();
        }
        else
        {
            addLineEdit->hide();
            saveButton->hide();
        }
    });

    connect(removeButton, &QPushButton::clicked, [this]{
        auto currentListData = jobListCurrentData();
        if(currentListData.isEmpty())
            return;
        QMessageBox msgBox(QMessageBox::Warning, tr("Do you want to remove"), tr("Do you want to remove '%0' from Job Type").arg(currentListData), QMessageBox::Yes | QMessageBox::No, this);
        if(msgBox.exec() == QMessageBox::Yes)
            io->dataEngine->removeJobTypeData(jobListCurrentData());

        updateData();
    });

    connect(saveButton, &QPushButton::clicked, [this]{
        io->dataEngine->insertJobTypeData(addLineEdit->text());

        addButton->toggle();

        updateData();
    });

    connect(editButton, &QPushButton::toggled, [this](bool toggled){
        if(toggled)
        {
            addButton->setChecked(false);
            addLineEdit->setText(jobListCurrentData());
            addLineEdit->show();
            modifyButton->show();
        }
        else
        {
            addLineEdit->clear();
            addLineEdit->hide();
            modifyButton->hide();
        }
    });

    connect(modifyButton, &QPushButton::clicked, [this]{
        io->dataEngine->insertJobTypeData(addLineEdit->text(), jobListCurrentData());

        editButton->toggle();

        updateData();
    });


}

void JobType::updateData()
{
    dataModel->clear();
    dataModel->setHorizontalHeaderLabels(QStringList()<< "Job Type Name");

    for(auto i: *io->dataEngine->getJobTypeSets())
        dataModel->appendRow(new QStandardItem(i));
}

QString JobType::jobListCurrentData()
{
    if(jobListView->currentIndex().row() > -1)
        return dataModel->item(jobListView->currentIndex().row())->text();
    return "";
}

