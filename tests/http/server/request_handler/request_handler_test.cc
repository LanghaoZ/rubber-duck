#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/request_handler/request_handler.h"
#include "http/server/request.h"
#include "http/server/reply.h"
#include "http/server/header.h"

namespace http {
namespace server {
namespace request_handler {

class mock_request_handler : public request_handler 
{
public:
  mock_request_handler(const std::string& location) 
    : request_handler(location)
  {
  }

  void handle_request(const request& req, reply& rep)
  {
  }
};

TEST(RequestHandlerTest, TargetBaseUrlIsAssigned) 
{
  mock_request_handler mock_request_handler("/base/");
  EXPECT_EQ(mock_request_handler.location_, "/base/");
}

TEST(RequestHandlerTest, NoURLTest) 
{
  mock_request_handler mock_request_handler("");
  EXPECT_TRUE(mock_request_handler.can_handle("/1"));
}

TEST(RequestHandlerTest, CanHandleRoot) 
{
  mock_request_handler mock_request_handler("/");
  EXPECT_TRUE(mock_request_handler.can_handle("/index.html"));
}

TEST(RequestHandlerTest, CanHandleNonRoot) 
{
  mock_request_handler mock_request_handler("/public/");
  EXPECT_TRUE(mock_request_handler.can_handle("/public/index.html"));
}

TEST(RequestHandlerTest, CannotHandleDifferentBaseUrl) 
{
  mock_request_handler mock_request_handler("/public/");
  EXPECT_FALSE(mock_request_handler.can_handle("/static/index.html"));
}

} // namespace request_handler
} // namespace server
} // namespace http