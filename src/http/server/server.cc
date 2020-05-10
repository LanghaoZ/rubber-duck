//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "http/server/server.h"
#include "http/server/session.h"
#include "http/server/request_handler/request_handler.h"
#include "logging/logging.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

server::server(short port)
  : io_service_(),
    signals_(io_service_),
    socket_(io_service_),
    acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
    session_manager_()
{

  // Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
  #if defined(SIGQUIT)
  signals_.add(SIGQUIT);
  #endif // defined(SIGQUIT)

  do_await_stop();

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

    // Check whether the server was stopped by a signal before this
    // completion handler had a chance to run.
    if (!acceptor_.is_open())
    {
      return;
    }

    if (!ec)
    {
      session_manager_.start(std::make_shared<session>(
        std::move(socket_), session_manager_));
      
    }

    do_accept();
  });
}

void server::do_await_stop()
{
  signals_.async_wait(
    [this](boost::system::error_code /*ec*/, int /*signo*/)
    {
      // The server is stopped by cancelling all outstanding asynchronous
      // operations. Once all operations have finished the io_service::run()
      // call will exit.
      acceptor_.close();
      session_manager_.stop_all();
      logging::logging::log_signal();
    });
}

} // namespace server
} // namespace http