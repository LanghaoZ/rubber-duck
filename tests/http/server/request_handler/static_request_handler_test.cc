#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "http/server/request_handler/static_request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
#include "http/server/header.h"
#include "http/server/request_parser.h"
#include <fstream>

namespace http {
namespace server {
namespace request_handler {

class StaticRequestHandlerTest : public ::testing::Test 
{
protected:
  StaticRequestHandlerTest()
    : handler_("/static/", "http/server/public")
  {
  }

  std::ifstream f_;
  static_request_handler handler_;
  request req_;
  reply rep_;
};

TEST_F(StaticRequestHandlerTest, DecodeSpaceCharacterInUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello%20world.html", result));
  EXPECT_EQ(result, "/hello world.html");
}

TEST_F(StaticRequestHandlerTest, DecodeExclamationMarkUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello%21world.html", result));
  EXPECT_EQ(result, "/hello!world.html");
}

TEST_F(StaticRequestHandlerTest, DecodePlusUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello%2Bworld.html", result));
  EXPECT_EQ(result, "/hello+world.html");
}

TEST_F(StaticRequestHandlerTest, DecodeQuestionMarkUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello%3Fworld.html", result));
  EXPECT_EQ(result, "/hello?world.html");
}

TEST_F(StaticRequestHandlerTest, DecodeHashMarkUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello%23world.html", result));
  EXPECT_EQ(result, "/hello#world.html");
}

TEST_F(StaticRequestHandlerTest, DecodePlusCharacterInUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello+world.html", result));
  EXPECT_EQ(result, "/hello world.html");
}

TEST_F(StaticRequestHandlerTest, DecodeNormalUrl) 
{
  std::string result;
  EXPECT_TRUE(handler_.url_decode("/hello_world.html", result));
  EXPECT_EQ(result, "/hello_world.html");
}

TEST_F(StaticRequestHandlerTest, DecodeInvalidCharacterInUrl) 
{
  std::string result;
  EXPECT_FALSE(handler_.url_decode("/hello%world.html", result));
}

TEST_F(StaticRequestHandlerTest, FindExtension) 
{
  EXPECT_EQ(handler_.find_file_extension("/hello.html"), "html");
}

TEST_F(StaticRequestHandlerTest, FindNoExtension) 
{
  EXPECT_EQ(handler_.find_file_extension("hello"), "");
}

TEST_F(StaticRequestHandlerTest, TranslateRequestPath) 
{
  EXPECT_EQ(handler_.translate_request_path("/static/index.html"), "http/server/public/index.html");
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPathWithInvalidCharacter) 
{
  req_.uri = "/hello%world/index.html";
  std::string request_path;
  EXPECT_FALSE(handler_.preprocess_request_path(req_, rep_, request_path));
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPathWithDotDot)
{
  req_.uri = "/../index.html";
  std::string request_path;
  EXPECT_FALSE(handler_.preprocess_request_path(req_, rep_, request_path));
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPathThatEndsWithSlash) 
{
  req_.uri = "/static/";
  std::string request_path;
  EXPECT_TRUE(handler_.preprocess_request_path(req_, rep_, request_path));
  EXPECT_EQ(request_path, "/static/index.html");
}

TEST_F(StaticRequestHandlerTest, PreprocessRequestPath) 
{
  req_.uri = "/static/index.html";
  std::string request_path;
  EXPECT_TRUE(handler_.preprocess_request_path(req_, rep_, request_path));
  EXPECT_EQ(request_path, "/static/index.html");
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithValidFilePath) 
{
  req_.uri = "/static/text.txt";
  handler_.handle_request(req_, rep_);
  EXPECT_EQ(rep_.content, "rubber-duck");
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithInvalidFilePath) 
{
  req_.uri = "/static/does-not-exist.txt";
  handler_.handle_request(req_, rep_);
  EXPECT_EQ(rep_.status, reply::not_found);
}

TEST_F(StaticRequestHandlerTest, HandleRequestWithInvalidRequestPath) 
{
  req_.uri = "/static/hello%world.txt";
  handler_.handle_request(req_, rep_);
  EXPECT_EQ(rep_.status, reply::bad_request);
}

TEST_F(StaticRequestHandlerTest, StaticReqIndex) 
{
  req_.uri = "/static/index.html";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/index.html");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqHome) 
{
  req_.uri = "/static/home.html";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/home.html");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqTxt) 
{
  req_.uri = "/static/text.txt";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/text.txt");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqJpg) 
{
  req_.uri = "/static/rubber_duck.jpg";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/rubber_duck.jpg");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqZip) 
{
  req_.uri = "/static/rubber_duck.zip";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/rubber_duck.zip");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

TEST_F(StaticRequestHandlerTest, StaticReqNoFile) 
{
  req_.uri = "/static/notexist.file";
  handler_.handle_request(req_, rep_);
  EXPECT_EQ(rep_.status, reply::not_found);
}

TEST_F(StaticRequestHandlerTest, StaticReqFooFile) 
{
  req_.uri = "/static/file.foo";
  handler_.handle_request(req_, rep_);

  f_.open("http/server/public/file.foo");
  std::string content;
  char c;
  while (f_.get(c)) content += c;
  f_.close();
  
  EXPECT_EQ(rep_.status, reply::ok);
  EXPECT_EQ(rep_.content, content);
}

} // namespace request_handler
} // namespace server
} // namespace http