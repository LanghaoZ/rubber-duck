//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "http/server/server.h"
#include "http/server/session.h"
#include "http/server/request_handler.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

server::server(short port)
  : io_service_(),
    socket_(io_service_),
    acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
    request_handler_()
{
  do_accept();
}

void server::run()
{
  io_service_.run();
}

void server::do_accept()
{
  acceptor_.async_accept(socket_,
    [this](boost::system::error_code ec)
  {
    if (!ec)
    {
      std::make_shared<session>(std::move(socket_), request_handler_)->start();
    }

    do_accept();
  });
}

} // namespace server
} // namespace http