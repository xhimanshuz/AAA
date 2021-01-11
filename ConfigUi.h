#ifndef CONFIGUI_H
#define CONFIGUI_H

class IOHandler;

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QGroupBox>

class ConfigUI : public QDialog
{
    Q_OBJECT

    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;
    QLineEdit *pdfApplicationUrl;
    QPushButton *pdfChoose;
    QPushButton *saveButton;
    QPushButton *closeButton;

    IOHandler *io;
    void render();
    void signalSetup();
    void setValues();
    void saveValues();
public:
    ConfigUI(IOHandler *_io, QWidget *parent = nullptr);

};

#endif // CONFIGUI_H
