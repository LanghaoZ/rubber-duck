#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
#include "http/server/header.h"

class RequestHandlerTest : public ::testing::Test 
{
  protected:
    http::server::request_handler handler_;
    http::server::reply rep_;
};

TEST_F(RequestHandlerTest, AssignsRequestContentToResponseBody) 
{

  http::server::request req = {
    "POST",                                      // method
    "/",                                         // uri
    1,                                           // http_version_major
    1,                                           // http_version_major
    {                                            // headers
      { "Accept", "application/json, */*" },
      { "Accept-Encoding", "gzip, deflate" },
      { "Connection", "keep-alive" },
      { "Content-Length", "18" },
      { "Content-Type", "application/json" },
      { "Host", "localhost:8080" },
      { "User-Agent", "HTTPie/0.9.8" }
    }, 
    "{\"hello\": \"world\"}"                     // body
  };

  handler_.handle_request(req, rep_);

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

  EXPECT_EQ(rep_.content, expected);
}