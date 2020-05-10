#include <fstream>
#include "http/server/request_handler/static_request_handler.h"
#include "http/server/request.h"
#include "http/server/response.h"
#include "http/server/mime_types.h"
#include "http/server/header.h"
#include "logging/logging.h"
#include "nginx/config.h"
#include "nginx/location.h"

namespace http {
namespace server {
namespace request_handler {

std::shared_ptr<static_request_handler> static_request_handler::init(const nginx::config& config)
{
  std::vector<nginx::location> locations = config.get_locations();
  nginx::location location = locations[0];
  return std::make_shared<static_request_handler>(location.path, location.root);
}

static_request_handler::static_request_handler(const std::string& path, const std::string& root)
  : request_handler(path),
    root_(root)
{
}

response static_request_handler::handle_request(const request& req)
{

  response res;

  std::string request_path;
  if (!preprocess_request_path(req, res, request_path))
  {
    return res;
  }
  
  std::string extension = find_file_extension(request_path);
  std::string full_path = translate_request_path(request_path);

  // Open the file to send back.
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    res = response::stock_response(response::not_found);
    return res;
  }

  // Fill out the response to be sent to the client.
  res.code = response::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    res.body.append(buf, is.gcount());

  res.headers[header::field_name_type_as_string(header::content_length)] = std::to_string(res.body.size());
  res.headers[header::field_name_type_as_string(header::content_type)] = mime_types::extension_to_type(extension);
  
  return res;
}

bool static_request_handler::preprocess_request_path(const request& req, response& res, std::string& request_path)
{
  // decode url to path
  if (!url_decode(req.uri, request_path))
  {
    res = response::stock_response(response::bad_request);
    return false;
  }

  // Request path must be absolute and not contain ".."
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    res = response::stock_response(response::bad_request);
    return false;
  }

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {
    request_path += "index.html";
  }

  return true;

}

std::string static_request_handler::find_file_extension(const std::string& request_path)
{
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }
  return extension;
}

std::string static_request_handler::translate_request_path(const std::string& request_path)
{
  return root_ + request_path.substr(path_.size());
}

bool static_request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace request_handler
} // namespace server
} // namespace http