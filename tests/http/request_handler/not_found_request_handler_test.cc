#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/request_handler/not_found_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/header.h"

namespace http {
namespace request_handler {

class NotFoundRequestHandlerTest : public ::testing::Test 
{
protected:
  NotFoundRequestHandlerTest()
    : handler_()
  {
  }

  not_found_request_handler handler_;
  response res_;
};

TEST_F(NotFoundRequestHandlerTest, Generates404Response) 
{
  request::request req;
  res_ = handler_.handle_request(req);
  EXPECT_EQ(res_.code, status_code::not_found);
}

} // namespace request_handler
} // namespace http