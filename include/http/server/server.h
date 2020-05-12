#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <unordered_map> 
#include "request_handler/request_handler.h"
#include "session.h"
#include "session_manager.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

class server
{
public:
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  /// Construct a server with port
  server(short port);

  /// Run the server's io_service loop.
  void run();

  static int request_count;
  static std::map<std::string, std::map<int, int>> request_db;

private:
  /// Perform an asynchronous accept operation.
  void do_accept();

  /// Wait for a request to stop the server.
  void do_await_stop();
 
  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  /// Acceptor used to listen for incoming connections.
  tcp::acceptor acceptor_;

  /// The connection manager which owns all live connections.
  session_manager session_manager_;

  /// The next socket to be accepted.
  tcp::socket socket_;
  
};

} // namespace server
} // namespace http

#endif