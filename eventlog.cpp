#include "eventlog.h"

namespace EventLog {
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(id, "ID", unsigned int)

// The operator is used for regular stream formatting
// It will convert the enum severity codes into a readable string
std::ostream& operator<< (std::ostream& strm, severity_level level)
{
    static const char* strings[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

// These two functions will write to the log file when it is opened
// and when it is closed, respectively.
// This could be useful, for example, if you are writing to an XML
// file and want to automatically add the open and close tags
void
write_header(sinks::text_file_backend::stream_type& file)
{
    file << "*****  Heater Simulation Started *****" << std::endl;
}

void
write_footer(sinks::text_file_backend::stream_type& file)
{
    file << "***** Heater Simulation Concluded *****" << std::endl;
}

EventLog::EventLog()
{
    namespace fs = boost::filesystem;

     // Access the core
    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

     // Create a text file sink
    typedef boost::log::sinks::synchronous_sink<sinks::text_file_backend> file_sink;

     // check to see if the destination folder exists
    std::string targetFolder = "./EventLogs";
    fs::path dest(targetFolder);

    // if not, create it
    if (!fs::exists(dest) || !fs::is_directory(dest))
    {
        fs::create_directory(dest);
    }

     // Create the log file name in the form ./logs/Event_date_time_counter.log
    std::string filename = "Event";
    fs::path logPath = fs::absolute(targetFolder + "/");
    logPath = fs::absolute(logPath.string() + filename +  "_%Y%m%d_%H%M%S_%5N.log");

     // force file rotation after file reches 1 mb.
    boost::shared_ptr<file_sink> sink(new file_sink(
        keywords::file_name = logPath.string(),                  // the resulting file name pattern
        keywords::rotation_size =  1 * 1024 * 1024               // rotation size, in characters
        ));

     // Set up where the rotated files will be stored
    sink->locked_backend()->set_file_collector(
        sinks::file::make_collector(
            keywords::target = targetFolder,         // the target directory
            keywords::max_size = 1 * 1024 * 1024    // rotate files after 2 megabytes
        ));

    // Upon restart, scan the directory for files matching the file_name pattern and delete any required files
    sink->locked_backend()->scan_for_files();

     // Enable auto-flushing after each log record written
    sink->locked_backend()->auto_flush(true);

     // add common attributes like date and time.
    boost::log::add_common_attributes();

     // add an ID attribute. This is how the core will know to write only to
     // this log file, as set by the filter
    auto attr = attrs::constant<unsigned int>(99);
    m_logger.add_attribute("ID", attr);

    sink->set_filter( expr::attr<unsigned int>("ID") == 99 );


     // format of the log records when streamed
    sink->set_formatter(expr::stream
                         << "[" << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d %H:%M:%S.%f")
                         << "] |" << severity
                         << "| [" << expr::smessage
                         << "] ") ;

     // establish the header and footer methods
    sink->locked_backend()->set_open_handler(&write_header);
    sink->locked_backend()->set_close_handler(&write_footer);

     // Add the sink to the core
    core->add_sink(sink);
}


// Actually designate the message to the log
void
EventLog::LogEvent(const severity_level level, const char* message)
{
    BOOST_LOG_SEV(m_logger, level) << message;
}

}
