//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <csignal>
#include <boost/asio.hpp>
#include "http/server/server.h"
#include "http/server/session.h"
#include "http/server/request_handler_factory.h"
#include "http/server/request_handler.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "logging/logs.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {

    // argument check
    if (argc != 2)
    {
      std::cout << "Usage: server <nginx_config_file>" << std::endl;
      return 1;
    }
    // parse nginx file to get port number
    nginx::config_parser parser;
    nginx::config config;
    bool success = parser.parse(argv[1], &config);

    if (success)
    {
      std::cout << "Successfully parsed nginx config: " << std::string(argv[1]) << std::endl;
    }
    else
    {
      std::cout << "Failed to parse nginx config: " << std::string(argv[1]) << std::endl;
    }

    int port = config.get_port();
    if (port == nginx::err_not_found) 
    {
      std::cout << "Failed to find the port from " << std::string(argv[1]) << std::endl;
      return 1;
    }

    // create request handlers
    std::vector<std::shared_ptr<http::server::request_handler>> request_handlers = http::server::request_handler_factory::create_request_handlers(config);

    Logs::init(false);

    // run the server
    http::server::server s(port, request_handlers);
    Logs::log_trace("Server started on port " + std::to_string(port));

    s.run();
    
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << std::string(e.what()) << std::endl;
  }

  return 0;
}
