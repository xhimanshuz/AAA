#include "IOHandler/IOHandler.h"

IOHandler* IOHandler::instance = nullptr;

IOHandler::IOHandler()
{
    dataEngine = new DataEngine;
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
