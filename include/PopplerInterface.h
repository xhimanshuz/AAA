#ifndef POPPLERINTERFACE_H
#define POPPLERINTERFACE_H

#include <poppler/qt5/poppler-qt5.h>
#include <QObject>
#include <QPixmap>
#include <QPrinter>
#include <QLabel>
#include <QPainter>

class PopplerInterface:public QObject
{
    Q_OBJECT
public:
    explicit PopplerInterface(QObject *parent = nullptr);
    virtual ~PopplerInterface();


    QImage toImage(QString filename);
    QLabel *l;
    void printDocument(QPrinter *printer);
};

#endif // POPPLERINTERFACE_H
