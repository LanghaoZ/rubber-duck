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

request_handler::request_handler(std::string& target_base_url)
  : target_base_url(target_base_url)
{

}

bool request_handler::can_handle(std::string& url)
{
  if (url.find(target_base_url) == 0) 
  {
    return true;
  }

  return false;
}

} // namespace server
} // namespace http