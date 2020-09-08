#ifndef RELEASEORDER_H
#define RELEASEORDER_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "IOHandler/IOHandler.h"

class AddReleaseOrder: public QDialog
{
    Q_OBJECT

    int code;
    QLineEdit *roNo;
    QComboBox *mediaHousList;
    QComboBox *jobTypeList;
    QLineEdit *editionCentre;
    QLineEdit *sizeDuration;
    QLineEdit *guarantedPosition;
    QLineEdit *premium;
    QTextEdit *premiumRemark;
    QLineEdit *rate;
    QTextEdit *rateRemark;
    QDateEdit *date;
    QComboBox *clientList;
    QLineEdit *caption;
    QLineEdit *dateOfPublication;
    QLineEdit *totalSizeDuration;
    QTextEdit *remarks;
    QLineEdit *hsnCode;
    QLineEdit *amount;
    QLineEdit *netAmount;
    QComboBox *CGST;
    QLineEdit *cgstAmount;
    QComboBox *SGST;
    QLineEdit *sgstAmount;
    QComboBox *IGST;
    QLineEdit *igstAmount;
    QLineEdit *roAmount;
    QPushButton *save;
    QPushButton *clear;
    QVBoxLayout *mainLayout;

    IOHandler *io;

    void render();
    void setupSignal();
    QStringList toStringList();
    void setValues(const QStringList detailList);
public:
    explicit AddReleaseOrder(QWidget *parent = nullptr);
    AddReleaseOrder(const int roNo, QWidget *parent = nullptr);
    virtual ~AddReleaseOrder();

};

#endif // RELEASEORDER_H
