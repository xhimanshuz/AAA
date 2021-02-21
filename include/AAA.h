#ifndef AAA_H
#define AAA_H

class Configure;

#include <QMainWindow>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QSqlTableModel>
#include <QDateEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QCompleter>
#include <QToolButton>
#include <QDate>
#include <QGraphicsBlurEffect>
#include <QStatusBar>

#include "Log.h"

#include "IOHandler/IOHandler.h"
#include "JobType.h"
#include "MediaPaymentDetail.h"
#include "PaymentReceiptDetail.h"
#include "MediaBillDetail.h"
#include "ReleaseOrder/AddReleaseOrder.h"
#include "UserUI/UserWindow.h"
#include "ReleaseOrder/AddReleaseOrder.h"
#include "InvoiceWindow.h"
#include "ConfigUi.h"

class AAA : public QMainWindow
{
    Q_OBJECT

//    QMenuBar *menuBar;
    QToolBar *toolBar;
    QAction *newJobType;
    QAction *newMediaHouse;
    QAction *newClient;
    QAction *mediaPaymentButton;
    QAction *paymentReceipt;
    QAction *invoiceButton;
    QAction *mediaBill;
    QTableView *roTable;
    QSqlTableModel *roDataModel;
    QDateEdit *searchDateFrom;
    QDateEdit *searchDateTo;
    QToolButton *dateSearchButton;
    QToolButton *dateClearButton;
    QComboBox *roNo;
    QComboBox *mediaHouse;
    QComboBox *client;
    QComboBox *jobType;
    QPushButton *go;
    QPushButton *newRO;
    QAction *exit;
    QPushButton *printList;
    QPushButton *setting;
    QVBoxLayout *mainLayout;

    IOHandler *io;
    Configure *config;
    std::shared_ptr<spdlog::logger> log;

    void render();
    void setupSignals();
    void updateRender();

    void populateData();
    int getRoNumber(const QModelIndex &index);
    int getRoNumber();
    void appConfigure();
public:
    explicit AAA(QWidget *parent = nullptr);
    virtual ~AAA();

signals:

};

#endif // AAA_H
