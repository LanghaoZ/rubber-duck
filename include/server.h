#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session;

class server
{
public:
  server(boost::asio::io_service& io_service, short port);

private:
  void start_accept();
  void handle_accept(std::shared_ptr<session> new_session,
      const boost::system::error_code& error);
 
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

#endif