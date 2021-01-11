#include "IOHandler/IOHandler.h"

IOHandler* IOHandler::instance = nullptr;

IOHandler::IOHandler()
{
    dataEngine = std::make_shared<DataEngine>();
    sql = std::make_shared<SQLiteHandler>();
}

IOHandler::~IOHandler()
{

}

IOHandler *IOHandler::getInstance()
{
    if(!instance)
        instance = new IOHandler;
    return instance;
}
