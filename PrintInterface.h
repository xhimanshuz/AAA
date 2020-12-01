#ifndef PRINTINTERFACE_H
#define PRINTINTERFACE_H

#include <QPrinter>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>

class PrintInterface: public QObject
{
    Q_OBJECT
public:
    PrintInterface(QObject *parent = nullptr): QObject(parent)
    {
        printer = new QPrinter(QPrinter::PrinterMode::PrinterResolution);
        printer->setColorMode(QPrinter::ColorMode::Color);
        printer->setPageSize(QPrinter::A4);
        printer->setOutputFormat(QPrinter::PdfFormat);
//        printer->set

    }


    static PrintInterface* get()
    {
        if(!PrintInterface::instance)
            PrintInterface::instance = new PrintInterface;
        return PrintInterface::instance;
    }

    void print(QWidget *widget)
    {

//        QPrinter print(QPrinter::PrinterMode::HighResolution);
//        print.setColorMode(QPrinter::ColorMode::Color);
//        print.setPageSize(QPrinter::A4);
//        print.setOutputFormat(QPrinter::PdfFormat);
//        auto url = QFileDialog::getSaveFileUrl(this, "SelectFile",QUrl("/home/gnu/Downloads"), "(PDF (*.pdf)");
        QUrl url("/home/gnu/Downloads/aaa");
        printer->setOutputFileName(url.path()+".pdf");
        printer->setOrientation(QPrinter::Portrait);


        auto xscale = printer->pageRect().width()/double(widget->width());
        auto yscale = printer->pageRect().height()/double(widget->height());

        QPainter painter(printer);
//        painter.transform(print.paperRect().center());
        painter.scale(xscale, yscale);
        widget->render(&painter);
    }

    static PrintInterface *instance;
     QPrinter *printer;
};


#endif // PRINTINTERFACE_H
