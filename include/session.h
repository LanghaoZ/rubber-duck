#ifndef SESSION_H
#define SESSION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "request.h"
#include "request_parser.h"
#include "reply.h"
#include "header.h"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket);
  tcp::socket& socket();
  void start();

private:
  void do_read();
  void do_write();

  // TODO: refactor
  void handle_request();

  tcp::socket socket_;

  /// Buffer for incoming data.
  std::array<char, 8192> buffer_;

  http::server::request request_;
  http::server::request_parser request_parser_;
  http::server::reply reply_;


};

#endif