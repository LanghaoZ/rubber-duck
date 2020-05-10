#ifndef HTTP_STATIC_REQUEST_HANDLER_H
#define HTTP_STATIC_REQUEST_HANDLER_H

#include "request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class static_request_handler : public request_handler
{
public:

  static std::shared_ptr<static_request_handler> init(const nginx::config& config);

  static_request_handler(const std::string& path, const std::string& root);

  /// Handle a request and produce a response.
  virtual response handle_request(const request& req);

private:

  /// decode url special characteres
  bool url_decode(const std::string& in, std::string& out);

  /// validate url
  bool preprocess_request_path(const request& req, response& res, std::string& request_path);

  /// Determine the file extension.
  std::string find_file_extension(const std::string& request_path);

  /// translate request path
  std::string translate_request_path(const std::string& request_path);

  /// the root directory to serve 
  std::string root_;

};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_STATIC_REQUEST_HANDLER_H