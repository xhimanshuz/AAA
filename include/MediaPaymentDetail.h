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
#include <QAction>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QRegExp>
#include <QIntValidator>
#include <QLocale>

#include "IOHandler/IOHandler.h"
#include "CustomItemDelegate.h"

class MediaPaymentDetail : public QDialog
{
    Q_OBJECT

    int rono;
    QLineEdit *roNo;
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
    QLineEdit *roAmount;
    QTableWidget *paymentTable;
    QLineEdit *totalAmount;
    QLineEdit *balAmount;
    QPushButton *save;
    QPushButton *clear;
    QVBoxLayout *mainLayout;

    QAction *deleteRow;
    IOHandler *io;
    int latestMediaPaymentId;

    void render();
    void setupSignals();
    void setValues();
    QList<QStringList> toStringList();
    void populateData(QList<QStringList> list = {} );
    void insertNewRow();
    void setValidator();
    QPushButton *createPrintButton();
    QPushButton *createDeleteButton();
    QLineEdit *createLineEditNumber(const QString amount );
    bool isValidRow(const int row);
    void setTotalAmount();
    void printMediaPayment();
private slots:
    void cellChanged(int row, int column);

public:
//    explicit PaymentDetail(QWidget *parent = nullptr);
    explicit MediaPaymentDetail(QWidget *parent = nullptr, int roNo=-1);
    virtual ~MediaPaymentDetail();

signals:

};

#endif // PAYMENTDETAIL_H
