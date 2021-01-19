#ifndef CUSTOMITEMDELEGATE_H
#define CUSTOMITEMDELEGATE_H

#include <QModelIndex>
#include <QItemDelegate>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDate>
#include <QDateEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QStyleOptionViewItem>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QStyledItemDelegate>

#include "IOHandler/IOHandler.h"

namespace CustomItemDelegate
{
class Amount: public QItemDelegate
{
    Q_OBJECT

public:
    Amount(QObject *parent = nullptr){}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        lineEdit->setValidator(new QRegExpValidator(QRegExp("\\d*\\.?\\d+")));
        return lineEdit;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        auto value = index.model()->data(index, Qt::EditRole).toString();
        qobject_cast<QLineEdit*>(editor)->setText(value);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex &index) const override
    {
        editor->setGeometry(option.rect);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
        auto value = lineEdit->text();

        model->setData(index, value, Qt::EditRole);
    }

};

class Date: public QItemDelegate
{
    Q_OBJECT

public:
    Date(QObject *parent = nullptr) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        auto dateEdit = new QDateEdit(parent);
        dateEdit->setDisplayFormat("dd-MM-yyyy");
        return dateEdit;
    }

    void setEditorData(QWidget* editor, const QModelIndex &index) const override
    {
        auto value = index.model()->data(index, Qt::EditRole).toString();
        qobject_cast<QDateEdit*>(editor)->setDate(QDate::fromString(value, "dd-MM-yyyy"));
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        auto value = dateEdit->text();

        model->setData(index, value, Qt::EditRole);
    }
};

class Mode: public QItemDelegate
{
    Q_OBJECT
public:
    Mode(QObject *parent = nullptr){}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        auto comboBox = new QComboBox(parent);
        comboBox->addItems(QStringList()<< "CASH"<< "CHEQUE" << "NEFT / RTGS"<< "UPI");
        //            comboBox->setEditable(true);
        return comboBox;
    }

    void setEditorData(QWidget* editor, const QModelIndex &index) const override
    {
        auto value = index.model()->data(index, Qt::EditRole).toString();
        qobject_cast<QComboBox*>(editor)->setCurrentText(value);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        auto value = comboBox->currentText();
        if(value == "CASH")
        {
            model->setData(index.siblingAtColumn(4), "---", Qt::EditRole);
            model->setData(index.siblingAtColumn(5), "---", Qt::EditRole);
        }

        model->setData(index, value, Qt::EditRole);

    }

    void currentValueChanged(const QString currentValue)
    {

    }
};

class Label: public QItemDelegate
{
    Q_OBJECT
public:
    Label(QObject *parent = nullptr){}

    //             QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    //             {
    //                return new QLabel(parent);
    //             }

    //             void setEditorData(QWidget* editor, const QModelIndex &index) const override
    //             {
    //                 auto value = index.model()->data(index, Qt::EditRole).toString();
    //                 qobject_cast<QLabel*>(editor)->setText(value);

    //             }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        //                 auto label = qobject_cast<QLabel*>(editor);
        //                 auto value = label->text();

        //                 model->setData(index, value, Qt::EditRole);

    }
};

class Print: public QStyledItemDelegate
{
    Q_OBJECT

    QTableWidget *tableWidget;
    QPushButton *button;

public:
    enum class TYPE
    {
        MEDIA_PAYMENT = 100,
        PAYMENT_RECEIPT
    };
    Print::TYPE type;
    Print(Print::TYPE _type, QTableWidget *_tableWidget, QObject *parent = nullptr): type{_type}, tableWidget{_tableWidget}
    {
        button = new QPushButton("PRINT");
    }

    QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem &, const QModelIndex &) const override
    {
        auto printButton = new QPushButton("Print", parent);
        if(type == Print::TYPE::MEDIA_PAYMENT)
        {
            connect(printButton, &QPushButton::clicked, [this]
            {
                auto id = tableWidget->item(tableWidget->currentIndex().row(), 0)->text().toInt();

                //                    auto roList = io->sql->getROStringList(rono);
                auto mediaPaymentList = IOHandler::getInstance()->sql->getMediaPaymentStringList(id);
                //                    if(!mediaPaymentList.isEmpty())
                //                        PDFTronInterface::get()->printRO(roList, mediaPaymentList);
            });

        }

        if(type == Print::TYPE::PAYMENT_RECEIPT)
        {

        }
        return printButton;
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

    void setModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) const override
    {

    }

};

}

#endif // CUSTOMITEMDELEGATE_H
