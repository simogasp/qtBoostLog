#ifndef TEMPERATURELOG_H
#define TEMPERATURELOG_H

#include <iostream>
#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/filesystem.hpp>

namespace TemperatureLog {
namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

class TemperatureLog
{
public:
    TemperatureLog();
    void logTemperature(double set, double temperature);

private:

};

}
#endif // TEMPERATURELOG_H
