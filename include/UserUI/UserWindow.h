#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>

#include "IOHandler/DataEngine.h"
#include "NewUser.h"

class UserWindow : public QDialog
{
    Q_OBJECT

    USER_TYPE type;
    QVBoxLayout *mainLayout;
    QTableView *usersList;
    QLineEdit *searchEdit;
    QPushButton *searchButton;
    QPushButton *addNew;
    QPushButton *refreshList;
    QPushButton *exit;
    QStandardItemModel *modelData;

    NewUser *newUserWidget;

    void render();
    void setupSignal();
    void populateData();

    IOHandler *io;

    QMap<QString, QStringList> *userMap;

public:
    explicit UserWindow(USER_TYPE _type, QWidget *parent = nullptr);

signals:

};

#endif // USERWINDOW_H
