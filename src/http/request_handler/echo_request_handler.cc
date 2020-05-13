#include "http/request_handler/echo_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/server/server.h"
#include "nginx/config.h"
#include "nginx/location.h"
#include "http/status_code.h"

namespace http {
namespace server {
namespace request_handler {

echo_request_handler* echo_request_handler::init(const std::string& location_path, const nginx::config& config)
{
  return new echo_request_handler();
}

response echo_request_handler::handle_request(const request& req)
{

  response res;

  // set response status
  res.code = http::server::status_code::ok;

  //Adding these two lines will cause echo request handler test to fail
  server::request_count++;
  server::request_db[req.uri][status_code::ok]++;

  // set response content
  res.body = request_to_string(req);
  
  // set response header
  res.headers[header::field_name_type_as_string(header::content_type)] = header::field_value_type_as_string(header::text_plain);
  res.headers[header::field_name_type_as_string(header::content_length)] = std::to_string(res.body.size());
  
  return res;
}

} // namespace request_handler
} // namespace server
} // namespace http