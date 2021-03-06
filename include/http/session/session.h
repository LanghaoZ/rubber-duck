#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <unordered_map> 
#include "http/request/request.h"
#include "http/request/request_parser.h"
#include "http/response.h"
#include "http/header.h"
#include "http/request_handler/request_handler.h"

namespace http {
namespace session {

class session_manager;

class session : public std::enable_shared_from_this<session>
{
public:
  session(const session&) = delete;
  session& operator=(const session&) = delete;

  // Construct a connection with the given socket.
  explicit session(boost::asio::ip::tcp::socket socket, 
    session_manager& manager);

  // Start the first asynchronous operation for the connection.
  virtual void start();

  // Stop all asynchronous operations associated with the connection.
  virtual void stop();

  void set_buffer(boost::array<char, 8192>& buffer);

  // handler to be called when the read operation completes
  int handle_read(const boost::system::error_code& ec,
    std::size_t bytes_transferred);

private:
  // Perform an asynchronous read operation.
  void do_read();

  // Perform an asynchronous write operation.
  void do_write();

  // Read rest of the data from socket before closing it
  void read_leftover(const std::string& extra_data_read);

  // helper function for read_leftover
  void read_request_body(const std::string& extra_data_read, 
    std::function<std::string (size_t length)> reader);

  // find remote endpoint address
  std::string find_client_address();

  // Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  // The manager for this connection.
  session_manager& session_manager_;

  // Buffer for incoming data.
  boost::array<char, 8192> buffer_;

  // The incoming request.
  request::request request_;

  // The parser for the incoming request.
  request::request_parser request_parser_;

  // The response to be sent back to the client.
  response res_;

};

} // namespace session
} // namespace http

#endif