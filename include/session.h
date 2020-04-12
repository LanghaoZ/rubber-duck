#ifndef SESSION_H
#define SESSION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "utils/request.h"
#include "utils/request_parser.h"
#include "utils/reply.h"
#include "utils/header.h"

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
  session(boost::asio::io_service& io_service);
  tcp::socket& socket();
  void start();

private:
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);
  void do_read();
  void do_write();

  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];

  http::server::request request_;
  http::server::request_parser request_parser_;
  http::server::reply reply_;
};

#endif