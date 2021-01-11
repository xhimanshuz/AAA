#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "DataEngine.h"
#include "SQLiteHandler.h"

#include <QObject>

class IOHandler
{
public:
    IOHandler();
    ~IOHandler();
    static IOHandler *instance;
    static IOHandler* getInstance();

    std::shared_ptr<DataEngine> dataEngine;
    std::shared_ptr<SQLiteHandler> sql;
};

#endif // IOHANDLER_H
