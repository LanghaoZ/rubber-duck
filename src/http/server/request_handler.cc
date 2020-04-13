//
// request_handler.cc
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include "http/server/request_handler.h"
#include "http/server/reply.h"
#include "http/server/request.h"

namespace http {
namespace server {

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req, reply& rep, std::string& request_body)
{

  rep.status = http::server::reply::ok;
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Type";
  rep.headers[0].value = "text/plain";
  rep.headers[1].name = "Content-Length";

  rep.content = "";
  rep.content += req.method;
  rep.content += " ";
  rep.content += req.uri;
  rep.content += " ";

  if (req.http_version_minor == 0) 
  {
    rep.content += "HTTP/1.0";
  }
  else if (req.http_version_minor == 1)
  {
    rep.content += "HTTP/1.1";
  }

  rep.content += "\r\n";

  for (int i = 0; i < req.headers.size(); i++) {
    rep.content += req.headers[i].name;
    rep.content += ": ";
    rep.content += req.headers[i].value;
    rep.content += "\r\n";
  }
  rep.content += "\r\n";

  rep.content += request_body;

  rep.headers[1].value = std::to_string(rep.content.size());
}

void request_handler::read_request_body(
  const request& req, 
  boost::asio::ip::tcp::socket& socket, 
  std::array<char, 8192>& buffer, 
  char* start, 
  char* end,
  std::string& body)
{

  body = "";

  // check header to tell whether there exists request body
  int contentLength = 0;
  for (int i = 0; i < req.headers.size(); i++) 
  {
    if (req.headers[i].name.compare("Content-Length") == 0) 
    {
      contentLength = std::stoi(req.headers[i].value);
    }
  }

  if (contentLength > 0) 
  {
    // already read some bytes from request body
    if (start != end) 
    {
      // read the rest of the data from buffer if there is one
      body = std::string(start, end - start);
      contentLength -= body.size();
    }
  }

  if (contentLength > 0) {
    char* data = new char[contentLength];
    size_t length = socket.read_some(boost::asio::buffer(data, contentLength));
    body += std::string(data, data + length);
  }

}

} // namespace server
} // namespace http