#ifndef JOBTYPE_H
#define JOBTYPE_H

#include "IOHandler/IOHandler.h"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QSqlQueryModel>
#include <QStandardItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QSet>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

class JobType : public QDialog
{
    Q_OBJECT

    QTableView *jobListView;
    QSqlQueryModel *dataModel;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *editButton;
    QPushButton *saveButton;
    QPushButton *modifyButton;
    QLineEdit *addLineEdit;
    QVBoxLayout *mainLayout;


    void render();
    void signalSetup();
    void updateData();
    QString jobListCurrentData();

    IOHandler *io;
public:
    explicit JobType(QWidget *parent = nullptr);
    virtual ~JobType();
signals:

};

#endif // JOBTYPE_H
