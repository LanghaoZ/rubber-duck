#include "http/server/request_handler/not_found_request_handler.h"
#include "nginx/config.h"
#include "nginx/location.h"

namespace http {
namespace server {
namespace request_handler {

std::shared_ptr<not_found_request_handler> not_found_request_handler::init(const nginx::config& config)
{
  std::vector<nginx::location> locations = config.get_locations();
  nginx::location location = locations[0];
  return std::make_shared<not_found_request_handler>(location.path);
}

not_found_request_handler::not_found_request_handler(const std::string& path)
  : request_handler(path)
{
}

response not_found_request_handler::handle_request(const request& req)
{
  return http::server::response::stock_response(http::server::response::not_found);
}

} // namespace request_handler
} // namespace server
} // namespace http