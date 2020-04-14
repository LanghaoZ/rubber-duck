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
  server(short port);
  void run();

private:
  void do_accept();
 
  boost::asio::io_service io_service_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  request_handler request_handler_;
};

} // namespace server
} // namespace http

#endif