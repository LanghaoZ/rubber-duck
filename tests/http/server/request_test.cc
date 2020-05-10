#include "gtest/gtest.h"
#include "http/server/request.h"

namespace http {
namespace server {

class RequestTest : public ::testing::Test 
{
protected:
  request req_;
};

TEST_F(RequestTest, GetZeroContentLength) 
{
  EXPECT_EQ(req_.get_content_length(), 0);
}

TEST_F(RequestTest, GetNonZeroContentLength) 
{
  req_.headers["Host"] = "rubberduck.com";
  req_.headers["Content-Length"] = "10";
  EXPECT_EQ(req_.get_content_length(), 10);
}

TEST_F(RequestTest, GETString) 
{
  req_.method = request::http_get;
  req_.uri = "/";
  req_.http_version_major = 1;
  req_.http_version_minor = 1;

  std::string expected = 
    "GET / HTTP/1.1\r\n"
    "\r\n";
  EXPECT_EQ(req_.to_string(), expected);

}

TEST_F(RequestTest, ConvertsToString) 
{
  req_.method = request::http_post;
  req_.uri = "/";
  req_.http_version_major = 1;
  req_.http_version_minor = 1;
  req_.headers["Accept"] = "*/*";
  req_.body = "message";

  std::string expected = 
    "POST / HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "\r\n"
    "message";

  EXPECT_EQ(req_.to_string(), expected);

}

} // namespace server
} // namespace http