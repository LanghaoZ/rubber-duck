#include "http/method_type.h"

namespace http {
namespace server {

std::string method_type_as_string(method_type method)
{
  switch (method)
  {
    case http_get:    return "GET";
    case http_post:   return "POST";
    case http_put:    return "PUT";
    case http_delete: return "DELETE";
    default:          return "";
  }
}

method_type string_as_method_type(const std::string& method)
{
  if (method == "GET")    return http_get;
  if (method == "POST")   return http_post;
  if (method == "PUT")    return http_put;
  if (method == "DELETE") return http_delete;
  return http_invalid;
}

} // namespace server
} // namespace http