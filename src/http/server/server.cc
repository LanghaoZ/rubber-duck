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

server::server(const std::string& address, const std::string& port)
  : io_service_(),
    acceptor_(io_service_),
    socket_(io_service_),
    request_handler_()
{

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  do_accept();
}

void server::run()
{
  // The io_service::run() call will block until all asynchronous operations
  // have finished. While the server is running, there is always at least one
  // asynchronous operation outstanding: the asynchronous accept call waiting
  // for new incoming connections.
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