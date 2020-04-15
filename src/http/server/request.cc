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

} // namespace server
} // namespace http