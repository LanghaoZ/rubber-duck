#include <typeinfo>
#include "gtest/gtest.h"
#include "http/server/request_handler_factory.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"
#include "http/server/request_handler.h"
#include "http/server/echo_request_handler.h"
#include "http/server/static_request_handler.h"
#include "nginx/config_parser.h"

TEST(RequestHandlerFactoryTest, GeneratesOneEchoOneStaticRequestHandler) {

  nginx::config_parser parser;
	nginx::config config;

  parser.parse("http/server/configs/request_handler_factory_test_config", &config);

  std::vector<std::shared_ptr<http::server::request_handler>> request_handlers 
    = http::server::request_handler_factory::create_request_handlers(config);

  EXPECT_EQ(request_handlers.size(), 3);

  int num_echo_request_handler = 0;
  int num_static_request_handler = 0;

  for (int i = 0; i < request_handlers.size(); i++)
  {
    if (typeid(*request_handlers[i].get()) == typeid(http::server::echo_request_handler))
    {
      num_echo_request_handler++;
    }
    else if (typeid(*request_handlers[i].get()) == typeid(http::server::static_request_handler))
    {
      num_static_request_handler++;
    }
  }

  EXPECT_EQ(num_echo_request_handler, 1);
  EXPECT_EQ(num_static_request_handler, 2);
}