#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "http/request_handler/static_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/header.h"
#include "http/request/request_parser.h"
#include "http/status_code.h"
#include "http/server/server.h"

namespace http {
namespace request_handler {

class StaticRequestHandlerTest : public ::testing::Test 
{
protected:
  StaticRequestHandlerTest()
    : handler_("/static", "http/request_handler/public")
  {
  }

  std::ifstream f_;
  static_request_handler handler_;
  request::request req_;
  response res_;
};

TEST_F(StaticRequestHandlerTest, DecodeSpaceCharacterInUrl) 
{
  req_.uri = "/static/hello%20world.html"; // "hello world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok); 
}

TEST_F(StaticRequestHandlerTest, DecodeExclamationMarkUrl) 
{
  req_.uri = "/static/hello%21world.html"; // "hello!world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok); 
}

TEST_F(StaticRequestHandlerTest, DecodePlusUrl) 
{
  req_.uri = "/static/hello%2Bworld.html"; // "hello+world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, DecodeQuestionMarkUrl) 
{
  req_.uri = "/static/hello%3Fworld.html"; // "hello?world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, DecodeHashMarkUrl) 
{
  req_.uri = "/static/hello%23world.html"; // "hello#world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, DecodePlusCharacterInUrl) 
{
  req_.uri = "/static/hello+world.html"; // "hello world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, DecodeNormalUrl) 
{
  req_.uri = "/static/hello_world.html"; // "hello world.html"
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, DecodeInvalidUrl) 
{
  req_.uri = "/static/hello%world.html";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::bad_request);
}

TEST_F(StaticRequestHandlerTest, FindNoExtension) 
{
  req_.uri = "/static/hello";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, HandlesRequestWithDotDot) 
{
  req_.uri = "/../index.html";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::bad_request);
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPathThatEndsWithSlash) 
{
  req_.uri = "/static/";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPath) 
{
  req_.uri = "/static/index.html";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::ok);
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithValidFilePath) 
{
  req_.uri = "/static/text.txt";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.body, "rubber-duck");
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithInvalidFilePath) 
{
  req_.uri = "/static/does-not-exist.txt";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::not_found);
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithInvalidRequestPath) 
{
  req_.uri = "/static/hello%world.txt";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::bad_request);
}

TEST_F(StaticRequestHandlerTest, StaticReqIndex) 
{
  req_.uri = "/static/index.html";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/index.html");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqHome) 
{
  req_.uri = "/static/home.html";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/home.html");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqTxt) 
{
  req_.uri = "/static/text.txt";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/text.txt");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqJpg) 
{
  req_.uri = "/static/rubber_duck.jpg";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/rubber_duck.jpg");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqZip) 
{
  req_.uri = "/static/rubber_duck.zip";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/rubber_duck.zip");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqNoFile) 
{
  req_.uri = "/static/notexist.file";
  res_ = handler_.handle_request(req_);
  EXPECT_EQ(res_.code, status_code::not_found);
}

TEST_F(StaticRequestHandlerTest, StaticReqFooFile) 
{
  req_.uri = "/static/file.foo";
  res_ = handler_.handle_request(req_);

  f_.open("http/request_handler/public/file.foo");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, content);
}

} // namespace request_handler
} // namespace http