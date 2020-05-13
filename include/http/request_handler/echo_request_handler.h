#ifndef HTTP_ECHO_REQUEST_HANDLER_H
#define HTTP_ECHO_REQUEST_HANDLER_H

#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class echo_request_handler : public request_handler
{
public:
  /// Take config to construct a echo request handler.
  static echo_request_handler* init(const std::string& location_path, const nginx::config& config);
  
  virtual response handle_request(const request& req);
};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_ECHO_REQUEST_HANDLER_H