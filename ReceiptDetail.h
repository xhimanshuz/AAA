#ifndef RECEIPTDETAIL_H
#define RECEIPTDETAIL_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QSqlTableModel>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QLineEdit>

#include "IOHandler/IOHandler.h"

class ReceiptDetail : public QDialog
{
    Q_OBJECT

    int rono;
    QComboBox *roNo;
    QDateEdit *date;
    QComboBox *client;
    QLineEdit *caption;
    QLineEdit *dateOfPublicationTelecast;
    QLineEdit *totalSizeDuration;
    QLineEdit *premium;
    QLineEdit *amount;
    QComboBox *mediaHouse;
    QComboBox *jobType;
    QLineEdit *editionCentre;
    QLineEdit *sizeDuration;
    QLineEdit *guarantedPosition;
    QLineEdit *rate;
    QLineEdit *netAmount;
    QTableView *receiptTableView;
    QSqlTableModel *tableModel;
    QLineEdit *totalAmount;
    QLineEdit *balAmount;
    QPushButton *save;
    QPushButton *clear;
    QVBoxLayout *mainLayout;

    IOHandler *io;

    void render();
    void setupSignals();
    void setValues();
    QStringList toStringList();
    void populateTable();

private slots:
    void cellChanged(int row, int column);
public:
    ReceiptDetail(QWidget *parent = nullptr, int roNo=-1);
    virtual ~ReceiptDetail();
};

#endif // RECEIPTDETAIL_H
