#ifndef HTTP_STATIC_REQUEST_HANDLER_H
#define HTTP_STATIC_REQUEST_HANDLER_H

#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

class static_request_handler : public request_handler
{
public:

  /// Take config to construct a static request handler.
  static static_request_handler* init(const std::string& location_path, const nginx::config& config);

  /// Take path and root and to give proper response to static request.
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

  /// the location path the handler needs to translate to root
  std::string path_;

  /// the root directory to serve 
  std::string root_;

};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_STATIC_REQUEST_HANDLER_H