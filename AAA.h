#ifndef AAA_H
#define AAA_H

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>

#include "IOHandler/IOHandler.h"
#include "JobType.h"
#include "PaymentDetail.h"
#include "ReceiptDetail.h"
#include "MediaBillDetail.h"
#include "ReleaseOrder/AddReleaseOrder.h"
#include "UserUI/UserWindow.h"
#include "ReleaseOrder/AddReleaseOrder.h"
#include "GenerateBillWindow.h"

class AAA : public QMainWindow
{
    Q_OBJECT

//    QMenuBar *menuBar;
    QToolBar *toolBar;
    QAction *newJobType;
    QAction *newMediaHouse;
    QAction *newClient;
    QAction *payment;
    QAction *receipt;
    QAction *generateBill;
    QAction *mediaBill;
    QTableView *roTable;
    QStandardItemModel *roDataModel;
    QDateEdit *searchDateFrom;
    QDateEdit *searchDateTo;
    QComboBox *roInvNo;
    QComboBox *mediaHouse;
    QComboBox *client;
    QComboBox *jobType;
    QPushButton *go;
    QPushButton *newRO;
    QAction *exit;
    QPushButton *printList;
    QVBoxLayout *mainLayout;

    IOHandler *io;

    void render();
    void setupSignals();
    void updateRender();

    void populateData();
public:
    explicit AAA(QWidget *parent = nullptr);

    enum RO
    {
        RONO=0,
        MEDIAHOUSELIST,
        JOBTYPELIST,
        EDITIONCENTRE,
        SIZEDURATION,
        GUARANTEDPOSITION,
        PREMIUM,
        PREMIUMREMRK,
        RATE,
        RATEREMARK,
        DATE,
        CLIENTLIST,
        CAPTION,
        DATEOFPUBLICATION,
        TOTALSIZEDURATION,
        REMARK,
        HSNCODE,
        AMOUNT,
        NETAMOUNT,
        CGST,
        CGSTREMARK,
        SGST,
        SGSTREMARK,
        IGST,
        IGSTREMARK,
        ROAMOUNT
    };

signals:

};

#endif // AAA_H
