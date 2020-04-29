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
  Logs::init(false);
  Logs::log_info("Logger has been initiated");
  try
  {

    // argument check
    if (argc != 2) {
      Logs::log_fatal("Usage: server <nginx_config_file>");
      return 1;
    }
    // parse nginx file to get port number
    nginx::config_parser parser;
    nginx::config config;
    bool success = parser.parse(argv[1], &config);

    if (success) {
      Logs::log_trace("Successfully parsed Nginx config: " + std::string(argv[1]));
    } 
    
    else {
      Logs::log_error("Failed to parse Nginx config:" + std::string(argv[1]));
    }

    int port = config.get_port();
    if (port == nginx::err_not_found) {
      Logs::log_fatal("Failed to parse the port from " + std::string(argv[1]));
      return 1;
    }

    // create request handlers
    std::vector<std::shared_ptr<http::server::request_handler>> request_handlers = http::server::request_handler_factory::create_request_handlers(config);
    Logs::log_debug("Instantiated request handlers");

    // run the server
    http::server::server s(port, request_handlers);
    Logs::log_info("Server initiated on port " + std::to_string(port));

    s.run();
    Logs::log_info("Server has started.");
    
  }
  catch (std::exception& e)
  {
    Logs::log_fatal("Failed to start server, encountered exception: " + std::string(e.what()));
    Logs::log_warning("Terminating the server...");
  }

  return 0;
}
