#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
class RequestHandlerTest : public ::testing::Test {
  protected:
    http::server::request_handler handler;
    http::server::request req;
    http::server::reply rep;
};
TEST_F(RequestHandlerTest, AssignsRequestContentToResponseBody) {
  std::string request_body = "{\"hello\":\"world\"}";
  req.headers.resize(1);
  req.method = "GET";
  req.uri = "/";
  req.headers[0].name = "field-name";
  req.headers[0].value = "field-value";
  handler.handle_request(req, rep, request_body);
  std::string expected = "";
  expected += "GET / HTTP/1.0\r\n";
  expected += "field-name: field-value\r\n";
  expected += "\r\n";
  expected += "{\"hello\":\"world\"}";
  EXPECT_EQ(rep.content.compare(expected), 0);
}