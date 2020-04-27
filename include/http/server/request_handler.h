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

namespace http {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
{
public:

  request_handler(const std::string& target_base_url);

  bool can_handle(const std::string& url);

  /// Handle a request and produce a reply.
  virtual void handle_request(const request& req, reply& rep) = 0;

  // url base path to handle
  std::string target_base_url;

};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_H