#ifndef LOGGING_H
#define LOGGING_H

#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <csignal>

using boost::asio::ip::tcp;

namespace logging {

class logging 
{
public:
  logging() = delete;
  
  // Initialize logging by setting up proper attributes like size and file name to the log file
  static void init(const std::string& file_name);
  
  // Use BOOST_LOG_TRIVIAL to log trace
  static void log_trace(std::string msg);
  
  // Use BOOST_LOG_TRIVIAL to log debug
  static void log_debug(std::string msg);
  
  // Use BOOST_LOG_TRIVIAL to log info
  static void log_info(std::string msg);
  
  // Use BOOST_LOG_TRIVIAL to log warning
  static void log_warning(std::string msg);
  
  // Use BOOST_LOG_TRIVIAL to log error
  static void log_error(std::string msg);
  
  // Use BOOST_LOG_TRIVIAL to log fatal
  static void log_fatal(std::string msg);
  
  // Logging shut down information and give warning
  static void log_signal();
};

} // namespace logging

#endif // LOGGING_H