#ifndef LOG_H
#define LOG_H

#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <QString>
#include <QStringList>

inline std::string str(const QString str)
{
    return str.toStdString();
}

inline std::string str(const QStringList strList)
{
    std::string string;
    for(auto &str: strList)
        string += str.toStdString() + ", ";
    return "QStringList("+ string + ")";
}

#endif // LOG_H

