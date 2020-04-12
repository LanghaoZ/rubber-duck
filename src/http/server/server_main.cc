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
#include <boost/asio.hpp>
#include "http/server/server.h"
#include "http/server/session.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    // argument check
    if (argc != 2)
    {
      std::cerr << "Usage: server <nginx_config_file>\n";
      return 1;
    }

    // parse nginx file to get port number
    nginx::config_parser parser;
    nginx::config config;
    parser.parse(argv[1], &config);

    int port = config.get_port(&config);
    if (port == nginx::err_not_found) 
    {
      std::cerr << 
        "port not found\n"
        "Usage: server <nginx_config_file>\n";
      return 1;
    }

    // Initialise the server.
    const std::string address = "localhost";
    http::server::server s(address, std::to_string(port));
    std::cerr << "Server started on port " << port << std::endl;
    
    // Run the server until stopped.
    s.run();
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
