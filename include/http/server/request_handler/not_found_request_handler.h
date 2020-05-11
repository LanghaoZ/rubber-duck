#ifndef HTTP_NOT_FOUND_REQUEST_HANDLER_H
#define HTTP_NOT_FOUND_REQUEST_HANDLER_H

#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class not_found_request_handler : public request_handler
{
public:

  static std::shared_ptr<not_found_request_handler> init(const nginx::config& config);

  not_found_request_handler(const std::string& path);

  /// Handle a request and produce a response.
  virtual response handle_request(const request& req);

};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_NOT_FOUND_REQUEST_HANDLER_H