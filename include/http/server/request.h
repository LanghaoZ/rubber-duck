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
#include <unordered_map>
#include "header.h"

namespace http {
namespace server {

/// A request received from a client.
struct request
{

  // The HTTP method (GET, PUT, POST, etc)
  enum method_type
  {
    http_get,
    http_post,
    http_put,
    http_delete,
    unknown
  } method;

  /// The path of the request
  std::string uri;

  /// A map of headers, for convenient lookup ("Content-Type", "Cookie", etc)
  std::unordered_map<std::string, std::string> headers;

  /// The content of the request
  std::string body;

  int http_version_major;

  int http_version_minor;

  /// String representation of this request object
  std::string to_string() const;

  /// String representation of the first line of this request (e.g. GET / HTTP/1.1)
  std::string to_digest() const;

  /// Get the value of Content-Length haeder field
  size_t get_content_length() const;

  /// Convert method type to string
  static std::string method_type_as_string(method_type method);

  /// Convert string to method type
  static method_type string_as_method_type(const std::string& method);
  
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_H