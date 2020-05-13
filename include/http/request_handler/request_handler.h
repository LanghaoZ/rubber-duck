//
// request_handler.h
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <string>
#include <functional>
#include "http/response.h"
#include "http/request/request.h"
#include "nginx/config.h"

namespace http {
namespace server {
namespace request_handler {

/// The common handler for all incoming requests.
class request_handler
{
public:

  // All subclasses must implement a static construction method
  // static request_handler init(const nginx::config& config);  // passes the parsed block

  /**
   * Handle a request and produce a response.
   * All subclasses implement a method to process requests.
   */
  virtual response handle_request(const request& req) = 0;

};

} // namespace request_handler
} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_H