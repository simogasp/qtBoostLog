#include "temperaturelog.h"

namespace TemperatureLog {

BOOST_LOG_ATTRIBUTE_KEYWORD(set_point, "SetPoint", double)
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_id, "ID", unsigned int)

boost::log::attributes::mutable_constant<double> attrSetPoint(0);
boost::log::attributes::constant<int> attrID(1);

TemperatureLog::TemperatureLog()
{
    namespace fs = boost::filesystem;

     // Access the core
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    // boost::shared_ptr<src::logger> core = m_logger::get();

     // Create a text file sink
    typedef boost::log::sinks::synchronous_sink<sinks::text_file_backend> file_sink;

     // check to see if the destination folder exists
    std::string targetFolder = "./TemperatureLogs";
    fs::path dest(targetFolder);

    // if not, create it
    if (!fs::exists(dest) || !fs::is_directory(dest))
    {
        fs::create_directory(dest);
    }


     // Create the log file name in the form ./logs/Temperature_date_time_counter.log
    std::string filename = "Temperature";

    fs::path logPath = fs::absolute(targetFolder + "/");
    logPath = fs::absolute(logPath.string() + filename +  "_%Y%m%d_%H%M%S_%5N.log");

     // Automatically rotate files when they reach 2 mb
    boost::shared_ptr<file_sink> sink(new file_sink(
        keywords::file_name = logPath.string(),                  // the resulting file name pattern
        keywords::rotation_size =  2 * 1024 * 1024               // rotation size, in characters
        ));

     // Set up where the rotated files will be stored
    sink->locked_backend()->set_file_collector(
        sinks::file::make_collector(
            keywords::target = targetFolder,         // the target directory
            keywords::max_size = 2 * 1024 * 1024    // rotate files after 2 megabytes
        ));

     // Upon restart, scan the directory for files matching the file_name pattern and delete any required files
    sink->locked_backend()->scan_for_files();

     // Enable auto-flushing after each log record written
    sink->locked_backend()->auto_flush(true);

     // add common attributes like date and time.
    logging::add_common_attributes();

     // add attributes.  I will make the temperature set point an attribute to illustrate
     // dynamic attributes.
    attrSetPoint.set(0);
    logging::core::get()->add_global_attribute("SetPoint", attrSetPoint);
    logging::core::get()->add_global_attribute("LineID", attrs::counter<unsigned int>(1));

     // Set up this log as id number 1
    auto attr = attrs::constant<unsigned int>(1);
    logging::core::get()->add_global_attribute("ID", attr);

    sink->set_filter( expr::attr<unsigned int>("ID") == 1 );

    // create the format of the output record, temperature data separated by commas
    sink->set_formatter(expr::stream
                         <<  std::setw(7) << std::setfill('0') << line_id
                         << "," << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d,%H:%M:%S.%f")
                         << "," << expr::attr<double>("SetPoint")
                         << "," << expr::smessage
                        );

     // Add the sink to the core
    core->add_sink(sink);
}


// Write the temperature data to the file
void
TemperatureLog::logTemperature(double set, double temperature)
{
    src::logger lg;

    // update the set point attribute.
    attrSetPoint.set(set);
    BOOST_LOG(lg) << "temperature: " << temperature;
}

}
