#ifndef HTTP_ECHO_REQUEST_HANDLER_H
#define HTTP_ECHO_REQUEST_HANDLER_H

#include "request_handler.h"

namespace http {
namespace server {

class echo_request_handler : public request_handler
{
public:
  echo_request_handler(std::string& target_base_url);
  
  /// Handle a request and produce a reply.
  virtual void handle_request(const request& req, reply& rep);

};

} // namespace server
} // namespace http

#endif // HTTP_ECHO_REQUEST_HANDLER_H