#include "http/server/header.h"

namespace http {
namespace server {

std::string header::field_name_type_as_string(field_name_type type)
{
  switch (type) {
    case accept:          return "Accept";
    case accept_encoding: return "Accept-Encoding";
    case connection:      return "Connection";
    case host:            return "Host";
    case user_agent:      return "User-Agent";
    case content_length:  return "Content-Length";
    case content_type:    return "Content-Type";
    default:              return "unknown header field name type";
  }
}

std::string header::field_value_type_as_string(field_value_type type)
{
  switch (type) 
  {
    case text_plain: return "text/plain";
    case text_html:  return "text/html";
    default:         return "unknown header field value type";
  }
}

} // namespace server
} // namespace http
