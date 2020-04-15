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
  // asynchronously read some data from socket and store it into buffer
  socket_.async_read_some(boost::asio::buffer(buffer_),
    // handler to be called when the read operation completes
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
            // buffer may contain more data beyond end of request header
            // read more data from socket if necessary
            read_leftover(std::string(start, end - start));

            request_handler_.handle_request(request_, reply_);
            do_write();
          }
          else if (result == http::server::request_parser::bad)
          {
            // respond with 400 status
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

void session::read_leftover(const std::string& extra_data_read)
{
  auto self(shared_from_this());
  request_handler_.read_request_body(request_, extra_data_read,
    [this, self](size_t content_length_left)
    {
      char* data = new char[content_length_left];
      size_t length = socket_.read_some(boost::asio::buffer(data, content_length_left));
      std::string addtional_data = std::string(data, data + length);
      delete data;
      return addtional_data;
    });
}

} // namespace server
} // namespace http