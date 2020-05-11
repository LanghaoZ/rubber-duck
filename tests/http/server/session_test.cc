#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <unordered_map> 
#include "gtest/gtest.h"
#include "http/server/session.h"
#include "http/server/request_handler/request_handler.h"
#include "http/server/session_manager.h"
#include "nginx/config.h"
#include "nginx/config_parser.h"
#include "http/server/request_handler/request_handler_factory.h"
#include "nginx/location.h"

using namespace boost::system;

namespace http {
namespace server {

class SessionTest : public ::testing::Test {
protected:
  SessionTest() 
    : io_service_(), 
      socket_(io_service_)
  {
    request_handler::request_handler_factory::get_instance().init(std::vector<nginx::location>());
  }

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  session_manager manager_;
};

TEST_F(SessionTest, HandlesGoodRequest) 
{

  // instantiate session
  std::shared_ptr<session> s = std::make_shared<session>(std::move(socket_), manager_);
  
  // send a message to the server
  boost::array<char, 8192> buffer;
  std::string request = 
    "GET / HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "Connection: close\r\n\r\n";
  for (int i = 0; i < request.size(); i++)
  {
    buffer[i] = request[i];
  }
  s->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(s->handle_read(ec, request.size()), 0);
  
}

TEST_F(SessionTest, HandlesIntermediateRequest) 
{
  // instantiate session
  std::shared_ptr<session> s = std::make_shared<session>(std::move(socket_), manager_);
  
  // send a message to the server
  boost::array<char, 8192> buffer;
  std::string request = "GET / ";
  for (int i = 0; i < request.size(); i++)
  {
    buffer[i] = request[i];
  }
  s->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(s->handle_read(ec, request.size()), 0);
}

TEST_F(SessionTest, HandlesBadRequest) 
{

  // instantiate session
  std::shared_ptr<session> s = std::make_shared<session>(std::move(socket_), manager_);
  
  // send a message to the server
  boost::array<char, 8192> buffer;
  std::string request = "rubber-duck\r\n";
  for (int i = 0; i < request.size(); i++)
  {
    buffer[i] = request[i];
  }
  s->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(s->handle_read(ec, request.size()), 1);
}

TEST_F(SessionTest, HandlesErrorRequest) 
{
  // instantiate session
  std::shared_ptr<session> s = std::make_shared<session>(std::move(socket_), manager_);

  error_code ec = errc::make_error_code(errc::not_supported);
  EXPECT_EQ(s->handle_read(ec, 0), 1);
}

} // namespace server
} // namespace http