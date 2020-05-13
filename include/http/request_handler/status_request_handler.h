#ifndef HTTP_STATUS_REQUEST_HANDLER
#define HTTP_STATUS_REQUEST_HANDLER

#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class status_request_handler : public request_handler 
{
public:
  static status_request_handler* init(const std::string& location_path, const nginx::config& config);
  virtual response handle_request(const request& req);
};


} // namespace http
} // namespace server
} // namespace request_handler

#endif // HTTP_STATUS_REQUEST_HANDLER