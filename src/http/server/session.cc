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
#include "http/server/session_manager.h"
#include "logging/logs.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

session::session(boost::asio::ip::tcp::socket socket,
  session_manager& manager,
  std::vector<std::shared_ptr<request_handler>>& request_handlers,
  bool logging)
  : socket_(std::move(socket)),
    session_manager_(manager),
    request_handlers_(request_handlers),
    logging_(logging)
{
}

void session::start()
{
  if (logging_)
    //std::cout << "Started a session with " << socket_.remote_endpoint().address().to_string() << std::endl;
  do_read();
}

void session::stop()
{
  if (logging_)
    //std::cout << "Stopped a session with " << socket_.remote_endpoint().address().to_string() << std::endl;
  socket_.close();
}

void session::do_read()
{
  auto self(shared_from_this());
  // asynchronously read some data from socket and store it into buffer
  socket_.async_read_some(boost::asio::buffer(buffer_), 
    boost::bind(&session::handle_read, shared_from_this(),
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

int session::handle_read(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
  auto self(shared_from_this());
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

      if (logging_) {
        Logs::log_request(request_, socket_, true);
      }
      
      std::shared_ptr<request_handler> request_handler;
      if (find_request_handler(request_handler))
      {
        request_handler.get()->handle_request(request_, reply_);
      }
      else {
        // no request handler can handle the uri
        reply_ = http::server::reply::stock_reply(http::server::reply::not_found);
      }

      do_write();
      return 0;
    }
    else if (result == http::server::request_parser::bad)
    {
      if (logging_) {
        Logs::log_request(request_, socket_, false);
      } 
      
      // respond with 400 status
      reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
      do_write();
      return 1;
    }
    else
    {
      Logs::log_debug("Continue reading in HTTP request");
      do_read();
      return 0;
    }
  }
  else if (ec != boost::asio::error::operation_aborted)
  {
    Logs::log_error("Operation aborted. Session Stopped");
    session_manager_.stop(shared_from_this());
  }

  return 1;
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

      if (ec != boost::asio::error::operation_aborted)
      {
        session_manager_.stop(shared_from_this());
      }

    });
}

void session::read_leftover(const std::string& extra_data_read)
{
  auto self(shared_from_this());
  read_request_body(extra_data_read,
    [this, self](size_t content_length_left)
    {
      char* data = new char[content_length_left];
      size_t length = socket_.read_some(boost::asio::buffer(data, content_length_left));
      std::string addtional_data = std::string(data, data + length);
      delete data;
      return addtional_data;
    });
}

void session::read_request_body(const std::string& extra_data_read, 
  std::function<std::string (size_t length)> reader)
{
  size_t content_length_left = request_.get_content_length() - extra_data_read.size();
  std::string addtional_data = "";
  // read rest of the request body
  if (content_length_left > 0) 
  {
    Logs::log_debug("Reading in additional data");
    addtional_data += reader(content_length_left);
  }
  request_.body = extra_data_read + addtional_data;
}

bool session::find_request_handler(std::shared_ptr<request_handler>& request_handler)
{
  for (int i = 0; i < request_handlers_.size(); i++)
  {
    if (request_handlers_[i].get()->can_handle(request_.uri))
    {
      Logs::log_debug("Located request handler");
      request_handler = request_handlers_[i];
      return true;
    }
  }

  Logs::log_warning("Cannot find the corresponding request handler");
  return false;
}

void session::set_buffer(boost::array<char, 8192>& buffer)
{
  Logs::log_debug("Session read buffer has been initiated");
  buffer_ = buffer;
}

} // namespace server
} // namespace http