
#include <QApplication>

#include "AAA.h"
#include "Log.h"
//#include "ui_roform.h"


int main(int argc, char **argv)
{
    auto log = spdlog::daily_logger_st("dlog", "Logs/log");
    log->set_level(spdlog::level::debug);
    log->flush_on(spdlog::level::debug);

    try{
    QApplication app(argc, argv);
        AAA aaa;
        aaa.show();

    return app.exec();
    }
    catch(std::runtime_error &e)
    {
        log->error("std::runtime_error: {}", e.what());
    }
}
