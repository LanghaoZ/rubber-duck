#include "gtest/gtest.h"
#include "http/server/request_parser.h"
#include "http/server/request.h"


class RequestParserTest : public :: testing::Test{
protected:
  http::server::request_parser parser;
  http::server::request req;
  http::server::request_parser::result_type result;
};

TEST_F(RequestParserTest, GoodHTTP) {

  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::good;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, MissingSlash) {

  //missing first slash
  char input[1024] = "GET HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, WrongProtc) {

  //wrong protocal
  char input[1024] = "GET / SOMETHING/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;

  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, WrongVersionAfter) {

  //wrong version 1.i instead of 1.1
  char input[1024] = "GET / HTTP/1.i\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, WrongVersionBefore) {

  //wrong version i.1 instead of 1.1
  char input[1024] = "GET / HTTP/i.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, LackSlashN) {

  //wrong syntx after host
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\rConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::bad;
  EXPECT_TRUE(success);
}

TEST_F(RequestParserTest, DOUBLEREQUEST) {

  // the second request will be treated as request body
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\nGET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::good;
  EXPECT_TRUE(success);
}


TEST_F(RequestParserTest, TESTNOEND) {

  //Without end should be not complete
  char input[1024] = "GET / HTTP/1.1\r\nHost: www.rubberduck.com\r\nConnection: close";
  std::tie(result, std::ignore)  = parser.parse(req, input, input + strlen(input));
  bool success = result == http::server::request_parser::indeterminate;
  EXPECT_TRUE(success);
}
