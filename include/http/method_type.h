#ifndef HTTP_METHOD_TYPE_H
#define HTTP_METHOD_TYPE_H

#include <string>

namespace http {

enum method_type
{
  http_get,
  http_post,
  http_put,
  http_delete,
  http_invalid
};

// convert method type to string
std::string method_type_to_string(method_type method);

// convert string to method type
method_type string_to_method_type(const std::string& method);

} // namespace http

#endif // HTTP_REQUEST_H