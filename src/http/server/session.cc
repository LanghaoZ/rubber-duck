#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "http/server/session.h"
#include "http/server/request_parser.h"
#include "http/server/reply.h"
#include "http/server/request_handler.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

session::session(boost::asio::ip::tcp::socket socket,
  request_handler& handler)
  : socket_(std::move(socket)),
    request_handler_(handler)
{
}

void session::start()
{
  do_read();
}

void session::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
    [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
      {
        if (!ec)
        {
          char* start = buffer_.data();
          char* end = buffer_.data() + bytes_transferred;
          http::server::request_parser::result_type result;

          std::tie(result, start) = request_parser_.parse(
            request_, start, end);
          
          if (result == http::server::request_parser::good)
          {
            std::string request_body;
            request_handler_.read_request_body(request_, socket_, buffer_, start, end, request_body);
            request_handler_.handle_request(request_, reply_, request_body);
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

} // namespace server
} // namespace http