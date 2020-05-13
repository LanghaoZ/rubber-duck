//
// response.h
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "http/header.h"
#include "http/status_code.h"

namespace http {
namespace server {

/// A response to be sent to a client.
struct response
{
  /// The status of the response.
  status_code code;

  /// The headers to be included in the response.
  std::map<std::string, std::string> headers;

  /// The content to be sent in the response.
  std::string body;

};

/// Convert the response into a vector of buffers. The buffers do not own the
/// underlying memory blocks, therefore the response object must remain valid and
/// not be changed until the write operation has completed.
std::vector<boost::asio::const_buffer> response_to_buffers(const response& res);

/// Get a stock response.
response status_code_to_stock_response(const status_code& code);

} // namespace server
} // namespace http

#endif // HTTP_RESPONSE_H