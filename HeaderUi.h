#ifndef HEADERUI_H
#define HEADERUI_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>

class HeaderUi: public QWidget
{
    QString headingText;
    QString logoPath;
    QString color;
    QLabel *heading;
    QLabel *logo;
    QVBoxLayout *mainLayout;

    void render()
    {
        heading = new QLabel(QString("<h1>%0</h1>").arg(headingText), this);

        auto hbox = new QHBoxLayout;
        hbox->addWidget(heading);

        mainLayout->addLayout(hbox);
    }

    void styleSheetSetup()
    {
        setStyleSheet(QString("background-color: %0").arg(color));
        heading->setStyleSheet("QLabel{ color: white}");
    }

public:
    explicit HeaderUi(QString _heading, QString _color, QWidget *parent, QString _logo = ""): headingText(_heading), color(_color), QWidget(parent), logoPath(_logo)
    {
        mainLayout = new QVBoxLayout;
        render();
        setLayout(mainLayout);
        styleSheetSetup();
    }
    static QWidget* addHeader(QString _heading, QString _color, QWidget *parent, QString _logo)
    {
        return new HeaderUi(_heading, _color, parent, _logo);
    }

};

#endif // HEADERUI_H
