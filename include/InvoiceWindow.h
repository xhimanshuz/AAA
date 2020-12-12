#ifndef GENERATEBILLWINDOW_H
#define GENERATEBILLWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "AddInvoice.h"

class InvoiceWindow : public QDialog
{
    Q_OBJECT

    QComboBox *seachRoNo;
    QComboBox *mediaHouse;
    QComboBox *status;
    QPushButton *freshList;
    QVBoxLayout *mainLayout;
    QTableView *billListView;
    QSqlQueryModel *billDataModel;
    AddInvoice *addInvoice;

    void render();
    void setupSignals();
    void populateData();
    void filterWithClient(const QString _client);
    void filterWithStatus(const QString _status);
    void filterWithRO(const QString _ro);
    IOHandler *io;
    int islastSelectedMedia;

public:
    explicit InvoiceWindow(const QString _client="", const QStringList invno = {}, const int rono = 0, QWidget *parent = nullptr);
    virtual ~InvoiceWindow();

signals:

};

#endif // GENERATEBILLWINDOW_H
