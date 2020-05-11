#include "http/server/request_handler/echo_request_handler.h"
#include "http/server/request.h"
#include "http/server/response.h"
#include "http/server/server.h"
#include "nginx/config.h"
#include "nginx/location.h"

namespace http {
namespace server {
namespace request_handler {

std::shared_ptr<echo_request_handler> echo_request_handler::init(const nginx::config& config)
{
  std::vector<nginx::location> locations = config.get_locations();
  nginx::location location = locations[0];
  return std::make_shared<echo_request_handler>(location.path);
}

echo_request_handler::echo_request_handler(const std::string& path)
  : request_handler(path)
{
}

response echo_request_handler::handle_request(const request& req)
{

  response res;

  // set response status
  res.code = http::server::response::ok;

  //Adding these two lines will cause echo request handler test to fail
  server::request_count++;
  server::request_db[req.uri][response::ok]++;

  // set response content
  res.body = req.to_string();
  
  // set response header
  res.headers[header::field_name_type_as_string(header::content_type)] = header::field_value_type_as_string(header::text_plain);
  res.headers[header::field_name_type_as_string(header::content_length)] = std::to_string(res.body.size());
  
  return res;
}

} // namespace request_handler
} // namespace server
} // namespace http