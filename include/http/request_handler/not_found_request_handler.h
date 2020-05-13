#ifndef HTTP_NOT_FOUND_REQUEST_HANDLER_H
#define HTTP_NOT_FOUND_REQUEST_HANDLER_H

#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class not_found_request_handler : public request_handler
{
public:
  /// Take config as parameters to construct a not-found request handler.
  static not_found_request_handler* init(const std::string& location_path, const nginx::config& config);
  
  virtual response handle_request(const request& req);
};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_NOT_FOUND_REQUEST_HANDLER_H