#include "http/server/request.h"

namespace http {
namespace server {

size_t request::get_content_length()
{
  
  int contentLength = 0;
  for (int i = 0; i < headers.size(); i++) 
  {
    if (headers[i].name.compare(
      header::field_name_type_as_string(header::content_length)) == 0) 
    {
      contentLength = std::stoi(headers[i].value);
    }
  }

  return contentLength;
}

std::string request::to_string() const
{
  std::string res = "";
  res += method;
  res += " ";

  res += uri;
  res += " ";

  res += "HTTP/";
  res += std::to_string(http_version_major);
  res += ".";
  res += std::to_string(http_version_minor);

  res += "\r\n";

  for (int i = 0; i < headers.size(); i++) {
    res += headers[i].name;
    res += ": ";
    res += headers[i].value;
    res += "\r\n";
  }

  res += "\r\n";

  res += body;

  return res;
}

} // namespace server
} // namespace http