//
// request.h
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include "http/header.h"
#include "http/method_type.h"

namespace http {
namespace request {

// A request received from a client.
struct request
{

  // The HTML method (GET, PUT, POST, etc)
  method_type method;

  /// The path of the request
  std::string uri;

  // The HTTP version string as given in the request line, e.g. "HTTP/1.1"
  std::string version;

  // A map of headers, for convenient lookup ("Content-Type", "Cookie", etc)
  std::map<std::string, std::string> headers;

  /// The content of the request
  std::string body;

};

// string representation of this request object
std::string request_to_string(const request& req);

// string representation of the first line of this request (e.g. GET / HTTP/1.1)
std::string request_to_digest(const request& req);

// get the value of Content-Length haeder field
size_t get_request_content_length(const request& req);

} // namespace request
} // namespace http

#endif // HTTP_REQUEST_H