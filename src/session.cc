#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "session.h"
#include "utils/request_parser.h"
#include "utils/reply.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service)
  : socket_(io_service)
{
}

tcp::socket& session::socket()
{
  return socket_;
}

void session::start()
{
  // socket_.async_read_some(boost::asio::buffer(data_, max_length),
  //     boost::bind(&session::handle_read, this,
  //       boost::asio::placeholders::error,
  //       boost::asio::placeholders::bytes_transferred));
  do_read();
}

void session::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(data_),
      [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
      {
        if (!ec)
        {
          http::server::request_parser::result_type result;
          std::tie(result, std::ignore) = request_parser_.parse(
              request_, data_, data_ + bytes_transferred);

          if (result == http::server::request_parser::good)
          {
            //request_handler_.handle_request(request_, reply_);
            http::server::reply temp;
            temp.status = http::server::reply::ok;
            temp.content = std::string(data_, bytes_transferred);
            temp.headers.resize(2);
            temp.headers[0].name = "Content-Type";
            temp.headers[0].value = "text/plain";
            temp.headers[1].name = "Content-Length";
            temp.headers[1].value = std::to_string(bytes_transferred);
            reply_ = temp;
            do_write();
          }
          else if (result == http::server::request_parser::bad)
          {
            reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
            do_write();
          }
          else
          {
            do_read();
          }
        }
      });
}

void session::do_write()
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_, reply_.to_buffers(),
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if (!ec)
        {
          // Initiate graceful connection closure.
          boost::system::error_code ignored_ec;
          socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
            ignored_ec);
        }
      });
}




void session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
  if (!error)
  {
    //std::printf(data_);
    boost::asio::async_write(socket_,
        boost::asio::buffer(data_, bytes_transferred),
        boost::bind(&session::handle_write, this,
          boost::asio::placeholders::error));
  }
  else
  {
    delete this;
  }
}

void session::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    delete this;
  }
}