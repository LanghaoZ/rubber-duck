#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/request_handler/echo_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/header.h"

namespace http {
namespace request_handler {

class EchoRequestHandlerTest : public ::testing::Test 
{
protected:
  EchoRequestHandlerTest()
    : handler_()
  {
  }

  echo_request_handler handler_;
  response res_;
};

TEST_F(EchoRequestHandlerTest, GetEchoingTest) 
{
  request::request req;
  req.method = http_get;
  req.uri = "/";
  req.version = "HTTP/1.1";
  req.headers["Accept"] = "application/json, */*";
  req.headers["Accept-Encoding"] = "gzip, deflate";
  req.headers["Connection"] = "keep-alive";
  req.headers["Content-Type"] = "application/json";
  req.headers["Host"] = "localhost:8080";
  req.headers["User-Agent"] = "HTTPie/0.9.8";
  req.body = "";
  
  res_ = handler_.handle_request(req);
  
  std::string expected = 
    "GET / HTTP/1.1\r\n"
    "Accept: application/json, */*\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: keep-alive\r\n"
    "Content-Type: application/json\r\n"
    "Host: localhost:8080\r\n"
    "User-Agent: HTTPie/0.9.8\r\n\r\n";

  EXPECT_EQ(res_.body, expected);
}

TEST_F(EchoRequestHandlerTest, AssignsRequestContentToResponseBody) 
{
  request::request req;
  req.method = http_post;
  req.uri = "/";
  req.version = "HTTP/1.1";
  req.headers["Accept"] = "application/json, */*";
  req.headers["Accept-Encoding"] = "gzip, deflate";
  req.headers["Connection"] = "keep-alive";
  req.headers["Content-Length"] = "18";
  req.headers["Content-Type"] = "application/json";
  req.headers["Host"] = "localhost:8080";
  req.headers["User-Agent"] = "HTTPie/0.9.8";
  req.body = "{\"hello\": \"world\"}";
  
  res_ = handler_.handle_request(req);
  
  std::string expected = 
    "POST / HTTP/1.1\r\n"
    "Accept: application/json, */*\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: keep-alive\r\n"
    "Content-Length: 18\r\n"
    "Content-Type: application/json\r\n"
    "Host: localhost:8080\r\n"   
    "User-Agent: HTTPie/0.9.8\r\n"    
    "\r\n"
    "{\"hello\": \"world\"}";
  
  EXPECT_EQ(res_.body, expected);
}

} // namespace request_handler
} // namespace http