#ifndef HTTP_ECHO_REQUEST_HANDLER_H
#define HTTP_ECHO_REQUEST_HANDLER_H

#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class echo_request_handler : public request_handler
{
public:

  /// Take config to construct a echo request handler.
  static std::shared_ptr<echo_request_handler> init(const nginx::config& config);

  /// Take path and to give proper response to echo request.
  echo_request_handler(const std::string& path);

  /// Handle a request and produce a response.
  virtual response handle_request(const request& req);

};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_ECHO_REQUEST_HANDLER_H