#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <unordered_map> 
#include "http/request_handler/request_handler.h"
#include "http/session/session.h"
#include "http/session/session_manager.h"

namespace http {
namespace server {

class server
{
public:
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  // Construct a server with port
  server(short port);

  // Run the server's io_service loop.
  void run();

  static void update_request_history(const std::string& path, const status_code& code);
  static const std::map<std::string, std::map<status_code, int>>& get_request_db();
  static int get_request_count();

private:
  // Perform an asynchronous accept operation.
  void do_accept();

  // Wait for a request to stop the server.
  void do_await_stop();
 
  // The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  // The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  // Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  // The connection manager which owns all live connections.
  session::session_manager session_manager_;

  // The next socket to be accepted.
  boost::asio::ip::tcp::socket socket_;

  // request history
  static int request_count;
  static std::map<std::string, std::map<status_code, int>> request_db;
  
};

} // namespace server
} // namespace http

#endif