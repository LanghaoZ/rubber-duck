#ifndef LOGGING_H
#define LOGGING_H

#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <csignal>
#include "http/server/request.h"

using boost::asio::ip::tcp;

namespace logging {

class logging 
{
public:
  logging() = delete;
  static void init(const std::string& file_name);
  static void log_trace(std::string msg);
  static void log_debug(std::string msg);
  static void log_info(std::string msg);
  static void log_warning(std::string msg);
  static void log_error(std::string msg);
  static void log_fatal(std::string msg);
  static void log_signal();
};

} // namespace logging

#endif // LOGGING_H