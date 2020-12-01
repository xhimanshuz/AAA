#ifndef ROFORM_H
#define ROFORM_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

class ROForm : public QWidget
{
    Q_OBJECT

    QLabel icon;
    QLabel labelAddress;
    QLabel gstin;
    QLabel roDate;
    QLabel mediaHouse;
    QLabel mediaHouseAddress;
    QLabel rono;
    QLabel remark;
    QLabel jobtype;
    QLabel client;
    QLabel caption;
    QLabel editionCentre;
    QLabel dateOfPublication;
    QLabel sizeDuration;
    QLabel totalSizeDuration;
    QLabel guarantedPosition;
    QLabel premium;
    QLabel rate;
    QLabel rateRemark;
    QLabel amount;
    QLabel netAmount;
    QLabel CGST;
    QLabel cgstRate;
    QLabel SGST;
    QLabel sgstRate;
    QLabel IGST;
    QLabel igstRate;
    QLabel roAmount;
    QLabel date;
    QLabel leftAmount;
    QLabel mode;
    QLabel chequeNo;
    QLabel bankName;
    QLabel releaseOrderLogo;
    QVBoxLayout *mainLayout;
    QWidget *formWidget;
    void setValue(const QStringList detailList);
    void render();
    void setStyle();
public:
    explicit ROForm(const QStringList detailList, QWidget *parent = nullptr);


signals:

};

#endif // ROFORM_H
