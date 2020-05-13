#include "gtest/gtest.h"
#include "http/request/request.h"

namespace http {
namespace request {

class RequestTest : public ::testing::Test 
{
protected:
  request req_;
};

TEST_F(RequestTest, GetZeroContentLength) 
{
  EXPECT_EQ(get_request_content_length(req_), 0);
}

TEST_F(RequestTest, GetNonZeroContentLength) 
{
  req_.headers["Host"] = "rubberduck.com";
  req_.headers["Content-Length"] = "10";
  EXPECT_EQ(get_request_content_length(req_), 10);
}

TEST_F(RequestTest, GETString) 
{
  req_.method = http_get;
  req_.uri = "/";
  req_.version = "HTTP/1.1";

  std::string expected = 
    "GET / HTTP/1.1\r\n"
    "\r\n";
  EXPECT_EQ(request_to_string(req_), expected);

}

TEST_F(RequestTest, ConvertsToString) 
{
  req_.method = http_post;
  req_.uri = "/";
  req_.version = "HTTP/1.1";
  req_.headers["Accept"] = "*/*";
  req_.body = "message";

  std::string expected = 
    "POST / HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "\r\n"
    "message";

  EXPECT_EQ(request_to_string(req_), expected);

}

} // namespace request
} // namespace http