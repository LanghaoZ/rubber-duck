#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
#include "http/server/header.h"

class mock_request_handler : public http::server::request_handler 
{
public:
  mock_request_handler(const std::string& target_base_url) 
    : http::server::request_handler(target_base_url)
  {
  }

  void handle_request(const http::server::request& req, http::server::reply& rep)
  {
  }
};

TEST(RequestHandlerTest, TargetBaseUrlIsAssigned) {
  mock_request_handler mock_request_handler("/base/");
  EXPECT_EQ(mock_request_handler.target_base_url, "/base/");
}

TEST(RequestHandlerTest, CanHandleRoot) {
  mock_request_handler mock_request_handler("/");
  EXPECT_TRUE(mock_request_handler.can_handle("/index.html"));
}

TEST(RequestHandlerTest, CanHandleNonRoot) {
  mock_request_handler mock_request_handler("/public/");
  EXPECT_TRUE(mock_request_handler.can_handle("/public/index.html"));
}

TEST(RequestHandlerTest, CannotHandleDifferentBaseUrl) {
  mock_request_handler mock_request_handler("/public/");
  EXPECT_FALSE(mock_request_handler.can_handle("/static/index.html"));
}