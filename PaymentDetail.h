#ifndef PAYMENTDETAIL_H
#define PAYMENTDETAIL_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QLineEdit>
#include <QSqlTableModel>

#include "IOHandler/IOHandler.h"

class PaymentDetail : public QDialog
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
    QTableView *paymentTable;
    QSqlTableModel *paymentModel;
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
    void populateData();

private slots:
    void cellChanged(int row, int column);

public:
//    explicit PaymentDetail(QWidget *parent = nullptr);
    PaymentDetail(QWidget *parent = nullptr, int roNo=-1);
    virtual ~PaymentDetail();

signals:

};

#endif // PAYMENTDETAIL_H
