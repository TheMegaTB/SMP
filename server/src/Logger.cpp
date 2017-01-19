//
// Created by themegatb on 1/19/17.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Logger.h"

void Logger::log(LogLevel level, string msg, string customPrefix) {
    cout << Logger::getLevelPrefix(level, customPrefix) << "  " << msg << endl;
}

string Logger::getLevelPrefix(LogLevel level, string customPrefix) {
    string color;
    string prefix;
    switch (level) {
        case Error:
            color = ColorError;
            prefix = "       Error";
            break;
        case Warn:
            color = ColorWarn;
            prefix = "     Warning";
            break;
        case Info:
            color = ColorInfo;
            prefix = "        Info";
            break;
        case Debug:
            color = ColorDebug;
            prefix = "       Debug";
            break;
        case Trace:
            color = ColorTrace;
            prefix = "       Trace";
            break;
        case Custom:
            color = ColorCustom;
            stringstream tmp;
            tmp << setw(12) << customPrefix;
            prefix = tmp.str();
            break;
    }
    return color + Bold + prefix + Reset;
}
