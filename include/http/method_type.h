#ifndef HTTP_METHOD_TYPE_H
#define HTTP_METHOD_TYPE_H

#include <string>

namespace http {
namespace server {

enum method_type
{
  http_get,
  http_post,
  http_put,
  http_delete,
  http_invalid
};

// convert method type to string
std::string method_type_as_string(method_type method);

// convert string to method type
method_type string_as_method_type(const std::string& method);

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_H