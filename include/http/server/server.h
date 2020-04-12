#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "request_handler.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

class session;

class server
{
public:
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit server(const std::string& address, const std::string& port);

  /// Run the server's io_service loop.
  void run();

private:
  /// Perform an asynchronous accept operation.
  void do_accept();

  /// The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The next socket to be accepted.
  boost::asio::ip::tcp::socket socket_;

  /// The handler for all incoming requests.
  request_handler request_handler_;
};

} // namespace server
} // namespace http

#endif