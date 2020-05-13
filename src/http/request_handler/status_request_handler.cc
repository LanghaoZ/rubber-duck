#include "http/request_handler/status_request_handler.h"
#include "http/request_handler/request_handler_factory.h"
#include "http/server/server.h"
#include "http/request/request.h"
#include "http/response.h"
#include "nginx/config.h"
#include "nginx/location.h"
#include "http/status_code.h"

namespace http {
namespace server {
namespace request_handler {

status_request_handler* status_request_handler::init(const std::string& location_path, const nginx::config& config) 
{
  return new status_request_handler();
}

response status_request_handler::handle_request(const request& req) 
{

  response res;
  res.code = http::server::status_code::ok;

  server::request_count++;
  server::request_db[req.uri][status_code::ok]++;

  std::string request_breakdown = "";

  for (auto it_path = server::request_db.begin(); it_path != server::request_db.end(); ++it_path) {
    for (auto it_code = it_path->second.begin(); it_code != it_path->second.end(); ++it_code) {
      request_breakdown += "<tr><td>" + it_path->first + "</td><td>" + std::to_string(it_code->first) + "</td><td>" + std::to_string(it_code->second) + "</td></tr>";
    }
  }

  res.body = "<html><head>""<title>Server Status</title></head>"
    "<body><h2>Total number of requests</h1><div>" + std::to_string(server::request_count) + "</div>"
    "<h2>Detail Request Status</h2>"
    "<table>"
    "<tr><th>URL Requested</th><th>Return Code</th><th>Count</th></tr>" + request_breakdown + "</table>"
    "</body>"
    "</html>";

  res.headers[header::field_name_type_as_string(header::content_length)] = std::to_string(res.body.size());
  res.headers[header::field_name_type_as_string(header::content_type)] = header::field_value_type_as_string(header::text_html);

  return res;
}

}
}
}