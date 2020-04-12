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
#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;

server::server(boost::asio::io_service& io_service, short port)
  : socket_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  do_accept();
}

void server::do_accept()
{
  acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec)
  {
    if (!ec)
    {
      std::make_shared<session>(std::move(socket_))->start();
    }

    do_accept();
  });
}
