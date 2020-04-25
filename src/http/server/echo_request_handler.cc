#include "http/server/echo_request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"

namespace http {
namespace server {

echo_request_handler::echo_request_handler(std::string& target_base_url)
  : request_handler(target_base_url)
{

}

void echo_request_handler::handle_request(const request& req, reply& rep)
{

  // set response status
  rep.status = http::server::reply::ok;

  // set response header
  rep.headers.resize(2);
  rep.headers[0].name = header::field_name_type_as_string(header::content_type);
  rep.headers[0].value = header::field_value_type_as_string(header::text_plain);
  rep.headers[1].name = header::field_name_type_as_string(header::content_length);

  // set response content
  rep.content = req.to_string();
  
  // update content size in response header
  rep.headers[1].value = std::to_string(rep.content.size());
}

} // namespace server
} // namespace http