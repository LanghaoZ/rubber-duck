#ifndef HTTP_REQUEST_HANDLER_FACTORY_H
#define HTTP_REQUEST_HANDLER_FACTORY_H

#include <string>
#include <unordered_map> 
#include "http/request_handler/request_handler.h"
#include "nginx/config.h"

namespace http {
namespace request_handler {

/**
 * Singleton factory class that dispatches an appropriate request handler
 * given a request uri. 
 */
class request_handler_factory
{
public:
  request_handler_factory(request_handler_factory const&) = delete;
  void operator=(request_handler_factory const&) = delete;

  static request_handler_factory& get_instance()
  {
    static request_handler_factory instance;
    return instance;
  }

  // create request handlers needed for the server
  void init(const std::vector<nginx::location>& locations);

  // dispatch an appropriate request handler for a given request uri
  std::shared_ptr<request_handler> dispatch(const std::string& uri);

private:
  request_handler_factory();

  // config should contain one statement, which is the location directive.
  request_handler* create_handler(const std::string& location_path, 
    const std::string& location_handler, const nginx::config& config);

  // hash map that maps from location path to request handler
  std::unordered_map<std::string, std::shared_ptr<request_handler>> request_handlers_;
  
  // 404 handler
  std::shared_ptr<request_handler> http_404_request_handler_; 
};

} // namespace request_handler
} // namespace http

#endif // HTTP_REQUEST_HANDLER_FACTORY_H