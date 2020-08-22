#ifndef NEWUSER_H
#define NEWUSER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItem>

#include <IOHandler/IOHandler.h>

enum class USER_TYPE
{
    CLIENT,
    MEDIA_HOUSE
};

class NewUser : public QWidget
{
    Q_OBJECT

    USER_TYPE type;

    QVBoxLayout *mainLayout;
    QLabel *title;
    QLineEdit *userName;
    QLineEdit *contactPerson;
    QLineEdit *phone;
    QLineEdit *email;
    QTextEdit *address;
    QComboBox *city;
    QComboBox *state;
    QLineEdit *gstin;
    QLineEdit *pinCode;
    QPushButton *save;
    QPushButton *clear;

public:
    explicit NewUser(USER_TYPE type, QWidget *parent = nullptr);
    NewUser(QString username, QStringList userDetail, USER_TYPE _type, QWidget *parent = nullptr);
    virtual ~NewUser();
    void render();
    void setUpStyleSheet();
    void setValues(const QString client, const QStringList &strList);
    void setValues(const QList<QStandardItem*> &items);
    void signalSetup();
    void clearValues();
    bool isValid();
    const QStringList toStringList();

    QPushButton *getSaveButton() const;

signals:

};

#endif // NEWUSER_H
