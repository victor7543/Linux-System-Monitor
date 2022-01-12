#include <string>
#include <sstream>
#include "format.h"

using std::string;
using std::stringstream;

string FormatTimeUnit(long unit_value) {
    string formated_value = std::to_string(unit_value);

    if (formated_value.length() > 2) {
        formated_value = formated_value.substr(0, 2);
    }
    else if (formated_value.length() < 2)
    {
        formated_value.insert(0, "0");
    }
    return formated_value;
}

string Format::ElapsedTime(long total_seconds) {
    long hh, mm, ss;
    stringstream formated_time;
    hh = total_seconds / SECONDS_IN_ONE_HOUR;
    mm = (total_seconds % SECONDS_IN_ONE_HOUR) / 60;
    ss = total_seconds % 60;
    formated_time << FormatTimeUnit(hh) << ":" << FormatTimeUnit(mm) << ":" << FormatTimeUnit(ss);
    return formated_time.str();
}