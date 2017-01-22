//
// Created by themegatb on 1/19/17.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include "../include/Logger.hpp"

LogLevel stringToLogLevel(string level) {
    if (level == "error" || level == "Error" || level == "ERROR")
        return LogLevel::Error;
    else if (level == "warn" || level == "Warn" || level == "WARN")
        return LogLevel::Warn;
    else if (level == "custom" || level == "Custom" || level == "CUSTOM")
        return LogLevel::Custom;
    else if (level == "info" || level == "Info" || level == "INFO")
        return LogLevel::Info;
    else if (level == "debug" || level == "Debug" || level == "DEBUG")
        return LogLevel::Debug;
    else if (level == "trace" || level == "Trace" || level == "TRACE")
        return LogLevel::Trace;

    return DEFAULT_LOGLEVEL;
}

// TODO Show only above a certain level
void Logger::log(LogLevel level, string msg, string file, int line, string func, string customPrefix) {
    char *PATHS = getenv("PATHS");
    char *LOG = getenv("LOG");

    LogLevel minLevel = DEFAULT_LOGLEVEL;
    string showPaths = "";

    if (PATHS != NULL) showPaths = string(PATHS);
    if (LOG != NULL) minLevel = stringToLogLevel(LOG);

    if (level < minLevel) return;

    cout << Logger::getLevelPrefix(level, customPrefix) << "  " << msg << Reset << endl;
    if (showPaths.find("true") == 0) {
        // Cut the path down to the first occurence of src/ and print it indented w/ 1 more spaces than the prev. msg
        cout << "               " << DarkGrey
             << file.substr(file.find("src/"), file.length())
             << ":" << line
             << ":" << func
             << Reset << endl;
    }
}

string Logger::getLevelPrefix(LogLevel level, string customPrefix) {
    string prefixColor;
    string prefix;
    string textColor;
    switch (level) {
        case Error:
            prefixColor = ColorError;
            prefix = "       Error";
            break;
        case Warn:
            prefixColor = ColorWarn;
            prefix = "     Warning";
            break;
        case Info:
            prefixColor = ColorInfo;
            prefix = "        Info";
            break;
        case Debug:
            prefixColor = ColorDebug;
            prefix = "       Debug";
            textColor = Grey;
            break;
        case Trace:
            prefixColor = ColorTrace;
            prefix = "       Trace";
            textColor = DarkGrey;
            break;
        case Custom:
            prefixColor = ColorCustom;
            stringstream tmp;
            tmp << setw(12) << customPrefix;
            prefix = tmp.str();
            break;
    }
    return prefixColor + Bold + prefix + Reset + textColor;
}
