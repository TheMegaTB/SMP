//
// Created by themegatb on 1/19/17.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Logger.h"

void Logger::log(LogLevel level, string msg, string customPrefix) {
    cout << Logger::getLevelPrefix(level, customPrefix) << "  " << msg << Reset << endl;
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
