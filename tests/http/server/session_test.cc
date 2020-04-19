#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include "gtest/gtest.h"
#include "http/server/session.h"
#include "http/server/request_handler.h"

using namespace boost::system;

class SessionTest : public ::testing::Test {
  protected:
    SessionTest() 
      : io_service_(), 
        socket_(io_service_),
        handler_() 
    {

    }

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;
    http::server::request_handler handler_;
};

TEST_F(SessionTest, SessionStartsSuccessfully) {

  try 
  {
    std::make_shared<http::server::session>(std::move(socket_), handler_)->start();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    // the test should fail when there is an exception
    EXPECT_TRUE(false);
  }
  
}

TEST_F(SessionTest, HandlesGoodRequest) {

  // instantiate session
  auto session = std::make_shared<http::server::session>(std::move(socket_), handler_);
  
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
  session->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(session->handle_read(ec, request.size()), 0);
  
}

TEST_F(SessionTest, HandlesIntermediateRequest) {
  // instantiate session
  auto session = std::make_shared<http::server::session>(std::move(socket_), handler_);
  
  // send a message to the server
  boost::array<char, 8192> buffer;
  std::string request = "GET / ";
  for (int i = 0; i < request.size(); i++)
  {
    buffer[i] = request[i];
  }
  session->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(session->handle_read(ec, request.size()), 0);
}

TEST_F(SessionTest, HandlesBadRequest) {

  // instantiate session
  auto session = std::make_shared<http::server::session>(std::move(socket_), handler_);
  
  // send a message to the server
  boost::array<char, 8192> buffer;
  std::string request = "rubber-duck\r\n";
  for (int i = 0; i < request.size(); i++)
  {
    buffer[i] = request[i];
  }
  session->set_buffer(buffer);

  error_code ec;
  EXPECT_EQ(session->handle_read(ec, request.size()), 1);
}

TEST_F(SessionTest, HandlesErrorRequest) {
  // instantiate session
  auto session = std::make_shared<http::server::session>(std::move(socket_), handler_);

  error_code ec = errc::make_error_code(errc::not_supported);
  EXPECT_EQ(session->handle_read(ec, 0), 1);
}