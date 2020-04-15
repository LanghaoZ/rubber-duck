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

void request_handler::read_request_body(request& req, const std::string& extra_data_read, 
  std::function<std::string (size_t length)> reader)
{
  size_t content_length_left = req.get_content_length() - extra_data_read.size();
  std::string addtional_data = "";
  // read rest of the request body
  if (content_length_left > 0) 
  {
    addtional_data += reader(content_length_left);
  }
  req.body = extra_data_read + addtional_data;
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

  rep.content += "HTTP/";
  rep.content += std::to_string(req.http_version_major);
  rep.content += ".";
  rep.content += std::to_string(req.http_version_minor);

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