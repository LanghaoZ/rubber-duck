#ifndef HTTP_REQUEST_HANDLER_FACTORY_H
#define HTTP_REQUEST_HANDLER_FACTORY_H

#include <string>
#include <unordered_map> 
#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class request_handler_factory
{
public:
  static std::vector<std::shared_ptr<request_handler>> create_request_handlers(nginx::config& config);

private:
  request_handler_factory() {}
};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_FACTORY_H