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
#include "http/server/header.h"

namespace http {
namespace server {

request_handler::request_handler()
{
}

void request_handler::handle_request(const request& req, reply& rep)
{

  // set response status
  rep.status = http::server::reply::ok;

  // set response header
  rep.headers.resize(2);
  rep.headers[0].name = header::field_name_type_as_string(header::content_type);
  rep.headers[0].value = header::field_value_type_as_string(header::text_plain);
  rep.headers[1].name = header::field_name_type_as_string(header::content_length);

  // set response content
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

  rep.content += req.body;

  // update content size in response header
  rep.headers[1].value = std::to_string(rep.content.size());
}

} // namespace server
} // namespace http