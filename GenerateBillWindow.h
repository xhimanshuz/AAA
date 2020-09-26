#ifndef GENERATEBILLWINDOW_H
#define GENERATEBILLWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "AddGeneratebill.h"

class GenerateBillWindow : public QDialog
{
    Q_OBJECT

    QComboBox *seachRoNo;
    QComboBox *mediaHouse;
    QComboBox *status;
    QPushButton *freshList;
    QVBoxLayout *mainLayout;
    QTableView *billListView;
    QSqlQueryModel *billDataModel;
    AddGenerateBill *generateBill;

    void render();
    void setupSignals();
    void populateData();
    void filterWithClient(const QString _client);
    void filterWithStatus(const QString _status);

    IOHandler *io;
    int islastSelectedMedia;

public:
    explicit GenerateBillWindow(const QString _client="", const int invno = 0, const int rono = 0, QWidget *parent = nullptr);
    virtual ~GenerateBillWindow();

signals:

};

#endif // GENERATEBILLWINDOW_H
