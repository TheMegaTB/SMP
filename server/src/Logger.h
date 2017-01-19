//
// Created by themegatb on 1/19/17.
//

#ifndef SMARTHOME_LOGGER_H
#define SMARTHOME_LOGGER_H

#include <string>

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
    Error,
    Warn,
    Info,
    Debug,
    Trace,
    Custom
};

using namespace std;

class Logger {
    static string getLevelPrefix(LogLevel level, string customPrefix);

public:
    static void log(LogLevel level, string msg, string file, string customPrefix = "");
};

#define error(msg) { Logger::log(LogLevel::Error, msg, __FILE__); }
#define warn(msg) { Logger::log(LogLevel::Warn, msg, __FILE__); }
#define info(msg) { Logger::log(LogLevel::Info, msg, __FILE__); }
#define debug(msg) { Logger::log(LogLevel::Debug, msg, __FILE__); }
#define trace(msg) { Logger::log(LogLevel::Trace, msg, __FILE__); }
#define custom(prefix, msg) { Logger::log(LogLevel::Custom, msg, __FILE__, prefix); }


#endif //SMARTHOME_LOGGER_H
