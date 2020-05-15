#include <typeinfo>
#include "http/request_handler/status_request_handler.h"
#include "http/request_handler/request_handler_factory.h"
#include "http/server/server.h"
#include "http/request/request.h"
#include "http/response.h"
#include "nginx/config.h"
#include "nginx/location.h"
#include "http/status_code.h"

namespace http {
namespace request_handler {

status_request_handler* status_request_handler::init(const std::string& location_path, const nginx::config& config) 
{
  return new status_request_handler();
}

response status_request_handler::handle_request(const request::request& req) 
{

  response res;
  res.code = status_code::ok;

  std::string request_breakdown = "";
  std::string handler_breakdown = "";

  for (auto& it_path : server::server::get_request_db())
  {
    for (auto& it_code : it_path.second)
    {
      request_breakdown += "<tr><td>" + it_path.first + "</td><td>" + std::to_string(it_code.first) + "</td><td>" + std::to_string(it_code.second) + "</td></tr>";
    }
  }

  for (auto& it : request_handler_factory::get_instance().get_request_handler_names())
  {
    handler_breakdown += "<tr><td>" + it.first + "</td><td>" + it.second + "</td></tr>";
  }

  res.body = "<html><head><title>Server Status</title></head>"
    "<body><h2>Total number of requests</h1><div>" + std::to_string(server::server::get_request_count()) + "</div>"
    "<h2>Detailed Request Status</h2>"
    "<table>"
    "<tr><th>URL Requested</th><th>Return Code</th><th>Count</th></tr>" + request_breakdown + "</table>"
    "<table>"
    "<br />"
    "<tr><th>URL Path</th><th>Handler</th></tr>" + handler_breakdown +
    "</table>"
    "</body>"
    "</html>";

  res.headers[header::field_name_type_to_string(header::content_length)] = std::to_string(res.body.size());
  res.headers[header::field_name_type_to_string(header::content_type)] = header::field_value_type_to_string(header::text_html);

  return res;
}

} // namespace request_handler
} // namespace http