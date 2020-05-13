#include "http/request/request.h"

namespace http {
namespace request {

std::string request_to_string(const request& req)
{
  std::string res = request_to_digest(req);

  res += "\r\n";

  for (auto& it : req.headers)
  {
    res += it.first;
    res += ": ";
    res += it.second;
    res += "\r\n";
  }

  res += "\r\n";

  res += req.body;

  return res;
}

std::string request_to_digest(const request& req)
{
  std::string res = "";
  res += method_type_to_string(req.method);
  res += " ";

  res += req.uri;
  res += " ";

  res += req.version;

  return res;
}

size_t get_request_content_length(const request& req)
{
  
  int content_length = 0;

  auto it = req.headers.find(header::field_name_type_to_string(header::content_length));

  if (it != req.headers.end())
  {
    content_length = std::stoi(it->second);
  }

  return content_length;
}

} // namespace request
} // namespace http