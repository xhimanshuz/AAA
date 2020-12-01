#ifndef ADDGENERATEBILL_H
#define ADDGENERATEBILL_H

#include <QDialog>
#include <QPushButton>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QRegExp>
#include <QRegExpValidator>

#include "IOHandler/IOHandler.h"

class AddInvoice : public QWidget
{
    Q_OBJECT

    QLineEdit *roNo;
    QComboBox *invoiceNo;
    QDateEdit *date;
    QComboBox *clients;
    QLineEdit *grossAmt;
    QLineEdit *discountPerc;
    QLineEdit *discount;
    QLineEdit *netPayableAmount;
    QComboBox *cgstPerc;
    QLineEdit *CGST;
    QComboBox *sgstPerc;
    QLineEdit *SGST;
    QComboBox *igstPerc;
    QLineEdit *IGST;
    QLineEdit *invoiceAmount;
    QTextEdit *remark;
    QListView *totSizeDuration;
    QPushButton *save;
    QPushButton *remove;
    QPushButton *clear;
    QPushButton *print;
    QVBoxLayout *mainLayout;

    void render();
    void setupSignal();
    QStringList toStringList();
    void populateData();
    IOHandler *io;
    void setValidator();
public:
    explicit AddInvoice(QWidget *parent = nullptr, const int _invno = 0, const int rono = 0);
    virtual ~AddInvoice();

    void setValueFromNumbers(const QStringList invnoList);
    void setValue(const QStringList billList);
    void setValueFromRO(const QStringList roList);
    QComboBox *getClients() const;
    void clearValue();

signals:
    void saveClicked();
};

#endif // ADDGENERATEBILL_H
