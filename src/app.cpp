#include <QApplication>

#include "AAA.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    AAA aaa;
    aaa.show();

    return app.exec();
}
