#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/request_handler/status_request_handler.h"
#include "http/request/request.h"
#include "http/response.h"
#include "http/header.h"

namespace http {
namespace request_handler {

class StatusRequestHandlerTest : public ::testing::Test 
{
protected:
  StatusRequestHandlerTest()
    : handler_()
  {
  }

  status_request_handler handler_;
  response res_;
};

TEST_F(StatusRequestHandlerTest, GeneratesOkResponse) 
{
  request::request req;
  res_ = handler_.handle_request(req);
  EXPECT_EQ(res_.code, status_code::ok);
}

} // namespace request_handler
} // namespace http