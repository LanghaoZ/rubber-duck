#include <typeinfo>
#include "gtest/gtest.h"
#include "http/server/request_handler/request_handler_factory.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "http/server/request_handler/request_handler.h"
#include "http/server/request_handler/echo_request_handler.h"
#include "http/server/request_handler/static_request_handler.h"
#include "http/server/request_handler/not_found_request_handler.h"
#include "nginx/config_parser.h"
#include "nginx/location.h"

namespace http {
namespace server {
namespace request_handler {

TEST(RequestHandlerFactoryTest, GeneratesOneEchoOneStaticRequestHandler) 
{
  nginx::config_parser parser;
	nginx::config config;

  parser.parse("http/server/configs/request_handler_factory_test.conf", &config);

  request_handler_factory::get_instance().init(config.get_locations());

  auto echo_request_handler_ptr = request_handler_factory::get_instance().dispatch("/echo");
  EXPECT_EQ(typeid(*echo_request_handler_ptr), typeid(echo_request_handler));

  auto static_request_handler_ptr_1 = request_handler_factory::get_instance().dispatch("/static1/index.html");
  EXPECT_EQ(typeid(*static_request_handler_ptr_1), typeid(static_request_handler));
  
  auto static_request_handler_ptr_2 = request_handler_factory::get_instance().dispatch("/static2/does-not-exist.txt");
  EXPECT_EQ(typeid(*static_request_handler_ptr_2), typeid(static_request_handler));

  auto invalid_request_handler_ptr = request_handler_factory::get_instance().dispatch("/invalid_route");
  EXPECT_EQ(typeid(*invalid_request_handler_ptr), typeid(not_found_request_handler));
}

} // namespace request_handler
} // namespace server
} // namespace http