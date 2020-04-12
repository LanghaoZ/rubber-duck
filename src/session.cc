#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "session.h"
#include "request_parser.h"
#include "reply.h"

using boost::asio::ip::tcp;

session::session(tcp::socket socket)
  : socket_(std::move(socket))
{
}

tcp::socket& session::socket()
{
  return socket_;
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
          auto start = buffer_.data();
          auto end = buffer_.data() + bytes_transferred;
          http::server::request_parser::result_type result;
          std::tie(result, start) = request_parser_.parse(
              request_, start, end);
          
          if (result == http::server::request_parser::good)
          {

            // put everything that was read into reply content
            reply_.content = std::string(buffer_.data(), bytes_transferred);
            reply_.status = http::server::reply::ok;
            reply_.headers.resize(2);
            reply_.headers[0].name = "Content-Type";
            reply_.headers[0].value = "text/plain";
            
            // check header to tell whether there exists request body
            int contentLength = 0;
            size_t headerSize = 0;
            for (int i = 0; i < request_.headers.size(); i++) 
            {
              if (request_.headers[i].name.compare("Content-Length") == 0) 
              {
                contentLength = std::stoi(request_.headers[i].value);
              }
              headerSize += request_.headers[i].name.size();
              headerSize += request_.headers[i].value.size();
            }

            if (contentLength > 0) 
            {
              
              // already read some bytes from request body
              if (start != end) 
              {
                std::cout << "already read some bytes" << std::endl;
                int amountRead = end - start;
                contentLength -= amountRead;
              }

            }

            // read rest of the request body
            if (contentLength > 0) {
              char *data = new char[contentLength];
              size_t length = socket_.read_some(boost::asio::buffer(data, contentLength));
              std::string body(data, data + length);
              reply_.content += body;
            }
            
            reply_.headers[1].name = "Content-Length";

            // content length of the response is
            // size of request header + size of request body
            reply_.headers[1].value = std::to_string(reply_.content.size());

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

void session::handle_request()
{
}