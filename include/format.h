#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
const short SECONDS_IN_ONE_HOUR = 3600;
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif