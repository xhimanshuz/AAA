#ifndef MEDIABILL_H
#define MEDIABILL_H

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
#include <QStandardItemModel>
#include <QAction>

#include "IOHandler/IOHandler.h"

class MediaBill : public QDialog
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
    QTableWidget *mediaBillTableWidget;
    QLineEdit *totalAmount;
    QPushButton *save;
    QPushButton *clear;
    QVBoxLayout *mainLayout;

    IOHandler *io;
    QAction *deleteRow;

    void render();
    void setupSignals();
    void setValues();
    QList<QStringList> toStringList();
    void populateData(QList<QStringList> list = {});
    void insertNewRow();

private slots:
    void cellChanged(int row, int column);
public:
//    MediaBill(QWidget *parent = nullptr);
    MediaBill( QWidget *parent = nullptr, const int roNo=-1);

};

#endif // MEDIABILL_H
