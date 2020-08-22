#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "DataEngine.h"

#include <QObject>

class IOHandler
{
public:
    IOHandler();
    ~IOHandler();
    static IOHandler *instance;
    static IOHandler* getInstance();

    DataEngine *dataEngine;
};

#endif // IOHANDLER_H
