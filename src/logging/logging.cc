#include "logging/logging.h"

namespace logging {

void logging::init(const std::string& file_name) 
{
  boost::log::add_common_attributes();
  boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
  
  boost::log::add_file_log
  (
    boost::log::keywords::file_name = file_name,
    boost::log::keywords::rotation_size = 10 * 1024 * 1024,
    boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
    boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%",
    boost::log::keywords::auto_flush = true
  );

  boost::log::add_console_log(
    std::cout, 
    boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%]: %Message%"
  );

  boost::log::core::get()->set_filter
  (
      boost::log::trivial::severity >= boost::log::trivial::trace
  );
}

void logging::log_trace(std::string msg) 
{
    BOOST_LOG_TRIVIAL(trace) << msg;
}

void logging::log_debug(std::string msg) 
{
    BOOST_LOG_TRIVIAL(debug) << msg;
}

void logging::log_info(std::string msg) 
{
    BOOST_LOG_TRIVIAL(info) << msg;
}

void logging::log_warning(std::string msg) 
{
    BOOST_LOG_TRIVIAL(warning) << msg;
}

void logging::log_error(std::string msg) 
{
    BOOST_LOG_TRIVIAL(error) << msg;
}

void logging::log_fatal(std::string msg) 
{
    BOOST_LOG_TRIVIAL(fatal) << msg;
}

void logging::log_signal() 
{
    log_warning("Received termination signal. Cancelling all outstanding asynchronous operations...");
    log_warning("Shutting down the server...");
}

} // namespace logging