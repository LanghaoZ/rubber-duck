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
#include "http/server/request_handler/request_handler.h"
#include "http/server/response.h"
#include "http/server/request.h"
#include "http/server/header.h"

namespace http {
namespace server {
namespace request_handler {

request_handler::request_handler(const std::string& path)
  : path_(path)
{
}

} // namespace request_handler
} // namespace server
} // namespace http