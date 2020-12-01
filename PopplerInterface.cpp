#include "PopplerInterface.h"
#include <QPrintPreviewDialog>
#include <QEventLoop>

PopplerInterface::PopplerInterface(QObject *parent): QObject(parent)
{
    QPrinter *printer = new QPrinter;
    QPrintPreviewDialog preview(printer);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &PopplerInterface::printDocument);
    preview.exec();

}

PopplerInterface::~PopplerInterface()
{

}

QImage PopplerInterface::toImage(QString filename)
{
    Poppler::Document* document = Poppler::Document::load(filename);
    if (!document || document->isLocked()) {

      // ... error message ....

      delete document;
      return QImage();
    }
//    document->setRenderHint(Poppler::Document::Antialiasing);
    auto pdfPage = document->page(0);
    QPrinter p(QPrinter::PrinterMode::PrinterResolution);
    p.setPageSize(QPrinter::A4);
    auto pdfImg = pdfPage->renderToImage(72, 72, 10, 10, p.width(), p.height());

    l = new QLabel;
    l->setPixmap(QPixmap::fromImage(pdfImg));
//    l->show();

    return pdfImg;

}

void PopplerInterface::printDocument(QPrinter *printer)
{
    QPainter p(printer);

    QLabel label;
    label.setPixmap(QPixmap::fromImage(toImage("../Data/RO/ro.pdf")));

    label.render(&p);
}
