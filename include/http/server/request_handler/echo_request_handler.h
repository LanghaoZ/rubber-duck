#ifndef HTTP_ECHO_REQUEST_HANDLER_H
#define HTTP_ECHO_REQUEST_HANDLER_H

#include "request_handler.h"

namespace http {
namespace server {
namespace request_handler {

class echo_request_handler : public request_handler
{
public:
  echo_request_handler(const std::string& location);
  
  /// Handle a request and produce a reply.
  virtual void handle_request(const request& req, reply& rep);
};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_ECHO_REQUEST_HANDLER_H