#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "request.h"
#include "request_parser.h"
#include "reply.h"
#include "header.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

class request_handler;

class session : public std::enable_shared_from_this<session>
{
public:
  session(const session&) = delete;
  session& operator=(const session&) = delete;

  /// Construct a connection with the given socket.
  explicit session(boost::asio::ip::tcp::socket socket, request_handler& handler);

  /// Start the first asynchronous operation for the connection.
  void start();

  void set_buffer(boost::array<char, 8192>& buffer);

  /// handler to be called when the read operation completes
  int handle_read(const boost::system::error_code& ec,
    std::size_t bytes_transferred);

private:
  /// Perform an asynchronous read operation.
  void do_read();

  /// Perform an asynchronous write operation.
  void do_write();

  /// Read rest of the data from socket before closing it
  void read_leftover(const std::string& extra_data_read);

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The handler used to process the incoming request.
  request_handler& request_handler_;

  /// Buffer for incoming data.
  boost::array<char, 8192> buffer_;

  /// The incoming request.
  request request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  reply reply_;
};

} // namespace server
} // namespace http

#endif