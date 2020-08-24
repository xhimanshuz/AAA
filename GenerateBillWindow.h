#ifndef GENERATEBILLWINDOW_H
#define GENERATEBILLWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "AddGeneratebill.h"

class GenerateBillWindow : public QDialog
{
    Q_OBJECT

    QLineEdit *seachRoNo;
    QComboBox *mediaHouse;
    QComboBox *status;
    QPushButton *freshList;
    QVBoxLayout *mainLayout;
    QTableView *billListView;
    QStandardItemModel *billDataModel;
    AddGenerateBill *generateBill;

    void render();
    void setupSignals();
    void populateData();

    IOHandler *io;

public:
    explicit GenerateBillWindow(QWidget *parent = nullptr);
    virtual ~GenerateBillWindow();

signals:

};

#endif // GENERATEBILLWINDOW_H
