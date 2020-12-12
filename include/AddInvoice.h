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
#include <QAction>
#include <QToolButton>
#include <QDoubleSpinBox>

#include "IOHandler/IOHandler.h"

class AddInvoice : public QWidget
{
    Q_OBJECT

    QLineEdit *roNo;
    QLineEdit *invoiceNoCurrent;
    QToolButton *invNoAdd;
    QToolButton *invNoRemove;
    QComboBox *invoiceNo;
    QDateEdit *date;
    QComboBox *clients;
    QLineEdit *grossAmt;
    QDoubleSpinBox *discountPerc;
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
    QLineEdit *totSizeDuration;
    QPushButton *save;
    QPushButton *remove;
    QPushButton *clear;
    QPushButton *print;
    QVBoxLayout *mainLayout;

    IOHandler *io;
    int latestInvoiceCode;

    void render();
    void setupSignal();
    QStringList toStringList();
    void populateData();
    void setValidator();
public:
    explicit AddInvoice(QWidget *parent = nullptr, const int _invno = 0, const int rono = 0);
    virtual ~AddInvoice();

    void setValueFromNumbers(const QStringList invnoList);
    void setValue(const QStringList billList);
    void setValueFromRO(const QStringList roList);
    void setDefaultValue();
    QComboBox *getClients() const;
    void clearValue();
    void addNewInvoice();
    void removeInvoice();

signals:
    void saveClicked();
    void currentClientChanged(const QString partiesName);
};

#endif // ADDGENERATEBILL_H
