#include "gtest/gtest.h"
#include "http/server/reply.h"

class ReplyTest : public ::testing::Test {
  protected:
    http::server::reply reply;
};

TEST_F(ReplyTest, HandlesOkResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::ok);
  EXPECT_EQ(reply.status, http::server::reply::ok);
  EXPECT_EQ(reply.content, "");
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, "Content-Length");
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, "Content-Type");
  EXPECT_EQ(reply.headers[1].value, "text/html");
}

TEST_F(ReplyTest, HandlesBadRequestResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::bad_request);
  EXPECT_EQ(reply.status, http::server::reply::bad_request);
  EXPECT_EQ(reply.content, "<html><head><title>Bad Request</title></head><body><h1>400 Bad Request</h1></body></html>");
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, "Content-Length");
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, "Content-Type");
  EXPECT_EQ(reply.headers[1].value, "text/html");
}