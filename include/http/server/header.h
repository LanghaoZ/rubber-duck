//
// header.h
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include <string>

namespace http {
namespace server {

struct header
{
  /// possible field names of request/response header
  enum field_name_type 
  {
    accept,
    accept_encoding,
    connection,
    host,
    user_agent,
    content_length,
    content_type
  };
  
  /// convert type to proper strings
  static std::string field_name_type_as_string(field_name_type type);

  /// possible field values of request/response header
  enum field_value_type
  {
    text_plain,
    text_html
  };
  static std::string field_value_type_as_string(field_value_type type);
  
};

} // namespace server
} // namespace http

#endif // HTTP_HEADER_H