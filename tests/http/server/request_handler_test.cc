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
  // TODO: write a test for request handler
}