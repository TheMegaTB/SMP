//
// Created by themegatb on 1/19/17.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Logger.h"

// TODO Show only above a certain level
void Logger::log(LogLevel level, string msg, string file, int line, string func, string customPrefix) {
    cout << Logger::getLevelPrefix(level, customPrefix) << "  " << msg << Reset << endl;
    if (true) {
        cout << file << endl;
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
