#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/request/request_parser.h"
#include "http/request/request.h"

namespace http {
namespace request {

class RequestParserTest : public ::testing::Test
{
protected:
  request_parser parser_;
  request req_;
  request_parser::result_type result_;
};

TEST_F(RequestParserTest, GoodHTTP) 
{
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::good);
}

TEST_F(RequestParserTest, MissingSlash) 
{
  // missing first slash
  char input[1024] = "GET HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, WrongProtc) 
{
  // wrong protocal
  char input[1024] = "GET / SOMETHING/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, WrongVersionAfter) 
{
  // wrong version 1.i instead of 1.1
  char input[1024] = "GET / HTTP/1.i\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, WrongVersionBefore) 
{
  // wrong version i.1 instead of 1.1
  char input[1024] = "GET / HTTP/i.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, LackSlashN) 
{
  // wrong syntx after host
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\rConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, DoubleRequest) 
{
  // the second request will be treated as request body
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\nGET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::good);
}

TEST_F(RequestParserTest, MinorErrorinProtocal) 
{
  // add small error to HTTP to see if it can be detected
  char input[1024] = "GET / HTTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, ErrBeforeClose) 
{
  // add /n before close
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: \nclose\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, OneMoreNBeforeProtocal) 
{
  // add \n before host one more time
  char input[1024] = "GET / HTTP/1.1\r\n\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}


TEST_F(RequestParserTest, TestNoEnd) 
{
  // Without end should be not complete
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::indeterminate);
}

TEST_F(RequestParserTest, LargeHTTPMajorVersion) 
{
  char input[1024] = "GET / HTTP/10.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::good);
}

TEST_F(RequestParserTest, LargeHTTPMinorVersion) {
  char input[1024] = "GET / HTTP/1.10\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::good);
}

TEST_F(RequestParserTest, HeaderFieldNameHasEmptySpace) 
{
  char input[1024] = "GET / HTTP/1.1\r\nHos t: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::bad);
}

TEST_F(RequestParserTest, HasRequestBody) 
{
  char input[1024] = "POST / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n{\"hello\": \"world\"}";
  std::tie(result_, std::ignore) = parser_.parse(req_, input, input + strlen(input));
  EXPECT_EQ(result_, request_parser::good);
}

} // namespace request
} // namespace http