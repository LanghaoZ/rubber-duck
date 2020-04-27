#include <fstream>
#include "http/server/static_request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
#include "http/server/mime_types.h"
#include "http/server/header.h"

namespace http {
namespace server {

static_request_handler::static_request_handler(const std::string& target_base_url, const std::string& root)
  : request_handler(target_base_url),
    root_(root)
{

}

void static_request_handler::handle_request(const request& req, reply& rep)
{

  std::string request_path;
  if (!preprocess_request_path(req, rep, request_path))
  {
    return;
  }
  
  std::string extension = find_file_extension(request_path);
  std::string full_path = translate_request_path(request_path);

  // Open the file to send back.
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    rep = reply::stock_reply(reply::not_found);
    return;
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
    rep.content.append(buf, is.gcount());
  rep.headers.resize(2);
  rep.headers[0].name = header::field_name_type_as_string(header::content_length);
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = header::field_name_type_as_string(header::content_type);
  rep.headers[1].value = mime_types::extension_to_type(extension);
}

bool static_request_handler::preprocess_request_path(const request& req, reply& rep, std::string& request_path)
{
  // decode url to path
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return false;
  }

  // Request path must be absolute and not contain ".."
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
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
  return root_ + request_path.substr(target_base_url.size() - 1);
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

} // namespace server
} // namespace http