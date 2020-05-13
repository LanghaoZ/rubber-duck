#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "http/session/session.h"
#include "http/request/request_parser.h"
#include "http/response.h"
#include "http/request_handler/request_handler.h"
#include "http/session/session_manager.h"
#include "logging/logging.h"
#include "http/request_handler/request_handler_factory.h"
#include "http/status_code.h"

using boost::asio::ip::tcp;

namespace http {
namespace session {

session::session(boost::asio::ip::tcp::socket socket,
  session_manager& manager)
  : socket_(std::move(socket)),
    session_manager_(manager)
{
}

void session::start()
{
  do_read(); 
}

void session::stop()
{
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
    request::request_parser::result_type result;
    std::tie(result, start) = request_parser_.parse(
      request_, start, end);

    if (result == request::request_parser::good)
    {
      // buffer may contain more data beyond end of request header
      // read more data from socket if necessary
      read_leftover(std::string(start, end - start));

      logging::logging::log_info(request_to_digest(request_) + " FROM " + find_client_address() + "\n");
      logging::logging::log_trace(request_to_string(request_));
      
      std::shared_ptr<request_handler::request_handler> request_handler
        = request_handler::request_handler_factory::get_instance().dispatch(request_.uri);

      res_ = request_handler.get()->handle_request(request_);

      do_write();
      return 0;
    }
    else if (result == request::request_parser::bad)
    {
      logging::logging::log_info(request_to_digest(request_) + " FROM " + find_client_address() + "\n");
      logging::logging::log_trace(request_to_string(request_));

      // respond with 400 status
      res_ = status_code_to_stock_response(status_code::bad_request);
      do_write();
      return 1;
    }
    else
    {
      logging::logging::log_debug("Continue reading in HTTP request");
      do_read();
      return 0;
    }
  }
  else if (ec != boost::asio::error::operation_aborted)
  {
    logging::logging::log_warning("Closing session with " + find_client_address() + "\n");
    session_manager_.stop(shared_from_this());
  }

  return 1;
}

void session::do_write()
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_, response_to_buffers(res_),
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
  size_t content_length_left = get_request_content_length(request_) - extra_data_read.size();
  std::string addtional_data = "";
  // read rest of the request body
  if (content_length_left > 0) 
  {
    logging::logging::log_debug("Reading in additional data");
    addtional_data += reader(content_length_left);
  }
  request_.body = extra_data_read + addtional_data;
}

std::string session::find_client_address()
{
  std::string client_address;
  boost::system::error_code ec;
  boost::asio::ip::tcp::endpoint endpoint = socket_.remote_endpoint(ec);
  if (ec)
  {
    logging::logging::log_error("Failed to find the client address\n");
  }
  else 
  {
    client_address = endpoint.address().to_string();
  }

  return client_address;
}

void session::set_buffer(boost::array<char, 8192>& buffer)
{
  logging::logging::log_debug("Session read buffer has been initiated");
  buffer_ = buffer;
}

} // namespace session
} // namespace http