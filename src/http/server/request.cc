#include "http/server/request.h"

namespace http {
namespace server {

std::string request::method_type_as_string(method_type method)
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

request::method_type request::string_as_method_type(const std::string& method)
{
  if (method == "GET")    return http_get;
  if (method == "POST")   return http_post;
  if (method == "PUT")    return http_put;
  if (method == "DELETE") return http_delete;
  return unknown;
}

std::string request::to_string() const
{
  std::string res = to_digest();

  res += "\r\n";

  for (auto& it : headers)
  {
    res += it.first;
    res += ": ";
    res += it.second;
    res += "\r\n";
  }

  res += "\r\n";

  res += body;

  return res;
}

std::string request::to_digest() const
{
  std::string res = "";
  res += request::method_type_as_string(method);
  res += " ";

  res += uri;
  res += " ";

  res += "HTTP/";
  res += std::to_string(http_version_major);
  res += ".";
  res += std::to_string(http_version_minor);

  return res;
}

size_t request::get_content_length() const
{
  
  int content_length = 0;

  auto it = headers.find(header::field_name_type_as_string(header::content_length));

  if (it != headers.end())
  {
    content_length = std::stoi(it->second);
  }

  return content_length;
}

} // namespace server
} // namespace http