#include "http/request_handler/not_found_request_handler.h"
#include "nginx/config.h"
#include "nginx/location.h"
#include "http/server/server.h"
#include "http/status_code.h"

namespace http {
namespace request_handler {

not_found_request_handler* not_found_request_handler::init(const std::string& location_path, const nginx::config& config)
{
  return new not_found_request_handler();
}

response not_found_request_handler::handle_request(const request::request& req)
{
  server::server::update_request_history(req.uri, status_code::not_found);
  return status_code_to_stock_response(status_code::not_found);
}

} // namespace request_handler
} // namespace http