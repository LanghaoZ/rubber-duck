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

namespace http {
namespace server {

struct reply;
struct request;

/// The common handler for all incoming requests.
class request_handler
{
public:
  request_handler(const request_handler&) = delete;
  request_handler& operator=(const request_handler&) = delete;

  explicit request_handler();

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep, std::string& request_body);

  /// read request body if it exists
  void read_request_body(const request& req, 
    boost::asio::ip::tcp::socket& socket, 
    std::array<char, 8192>& buffer, 
    char* start, 
    char* end,
    std::string& body);

};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_H