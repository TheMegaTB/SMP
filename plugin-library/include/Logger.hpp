//
// Created by themegatb on 1/19/17.
//

#ifndef SMP_LOGGER_H
#define SMP_LOGGER_H

#include <string>

#define DEFAULT_LOGLEVEL LogLevel::Info

#define Reset        "\033[0m"
#define Bold         "\033[1m"
#define Grey         "\033[38;5;241m"
#define DarkGrey     "\033[38;5;239m"
#define ColorError   "\033[38;5;160m"
#define ColorWarn    "\033[38;5;214m"
#define ColorInfo    "\033[38;5;10m"
#define ColorDebug   "\033[38;5;244m"
#define ColorTrace   "\033[38;5;239m"
#define ColorCustom  "\033[38;5;14m"

enum LogLevel {
    Error = 3,
    Warn = 2,
    Custom = 1,
    Info = 0,
    Debug = -1,
    Trace = -2,
};

using namespace std;

class Logger {
    static string getLevelPrefix(LogLevel level, string customPrefix);

public:
    static void log(LogLevel level, string msg, string file, int line, string func, string customPrefix = "");
};

#define __CONTEXT__ __FILE__, __LINE__, __func__

#define error(msg) { Logger::log(LogLevel::Error, msg, __CONTEXT__); }
#define warn(msg) { Logger::log(LogLevel::Warn, msg, __CONTEXT__); }
#define info(msg) { Logger::log(LogLevel::Info, msg, __CONTEXT__); }
#define debug(msg) { Logger::log(LogLevel::Debug, msg, __CONTEXT__); }
#define trace(msg) { Logger::log(LogLevel::Trace, msg, __CONTEXT__); }
#define custom(prefix, msg) { Logger::log(LogLevel::Custom, msg, __CONTEXT__, prefix); }


#endif //SMP_LOGGER_H
