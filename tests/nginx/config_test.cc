#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nginx/config.h"
#include "nginx/config_parser.h"
#include "nginx/config_statement.h"

using ::testing::_;
using ::testing::Return;

namespace nginx {

class mock_config_statement : public config_statement 
{
public:
  MOCK_METHOD1(to_string, std::string(int));
};

TEST(ConfigTest, EmptyConfigTest) 
{
  std::unique_ptr<config> conf = std::make_unique<config>();
	EXPECT_EQ(conf->to_string(0), "");
}

TEST(ConfigTest, ToStringTest) 
{
  std::unique_ptr<config> conf = std::make_unique<config>();
  std::shared_ptr<mock_config_statement> conf_statement = std::make_shared<mock_config_statement>();

	conf->statements_.emplace_back(conf_statement);

	EXPECT_CALL(*conf_statement, to_string(_)).WillOnce(Return("server;"));
	EXPECT_EQ(conf->to_string(0), "server;");
}

TEST(ConfigTest, GetPortTestWithPort8080) 
{
  /**
   * server { 
   *   foo bar; 
   *   port 8080;
   * }
   */
  std::unique_ptr<config> inner_config = std::make_unique<config>();
  std::unique_ptr<config> outer_config = std::make_unique<config>();
	std::shared_ptr<config_statement> statement_foo_bar = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> statement_port_8080 = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> server_statement = std::make_shared<config_statement>();

	statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");
	statement_port_8080->tokens_.push_back("port");
	statement_port_8080->tokens_.push_back("8080");
	inner_config->statements_.emplace_back(statement_foo_bar);
	inner_config->statements_.emplace_back(statement_port_8080);
	server_statement->tokens_.push_back("server");
	server_statement->child_block_ = std::move(inner_config);
	outer_config->statements_.emplace_back(server_statement);

	EXPECT_EQ(outer_config->get_port(), 8080);
}

TEST(ConfigTest, GetPortTestWithoutPort) 
{
  /**
   * server { 
   *   foo bar;
   * }
   */
  std::unique_ptr<config> inner_config = std::make_unique<config>();
  std::unique_ptr<config> outer_config = std::make_unique<config>();
	std::shared_ptr<config_statement> statement_foo_bar = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> server_statement = std::make_shared<config_statement>();

	statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");
	inner_config->statements_.emplace_back(statement_foo_bar);
	server_statement->tokens_.push_back("server");
	server_statement->child_block_ = std::move(inner_config);
	outer_config->statements_.emplace_back(server_statement);

	EXPECT_EQ(outer_config->get_port(), -1);
}

TEST(ConfigTest, GetServersTest) 
{
  /**
   * server {
   *   handler static;
   *   location /static1/ {
   *     root ./public/public1;
   *   }
   *   location /static2/ {
   *     root ./public/public2;
   *   }
   * } 
   */
  std::unique_ptr<config> server_config = std::make_unique<config>();
  std::unique_ptr<config> server_child_config = std::make_unique<config>();
  std::unique_ptr<config> root1_config = std::make_unique<config>();
  std::unique_ptr<config> root2_config = std::make_unique<config>();
  std::shared_ptr<config_statement> root1_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> root2_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> location1_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> location2_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> handler_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> server_statement = std::make_shared<config_statement>();

  root1_statement->tokens_.push_back("root");
  root1_statement->tokens_.push_back("./public/public1");
  
  root2_statement->tokens_.push_back("root");
  root2_statement->tokens_.push_back("./public/public2");

  root1_config->statements_.emplace_back(root1_statement);
  root2_config->statements_.emplace_back(root2_statement);

  location1_statement->tokens_.push_back("location");
  location1_statement->tokens_.push_back("/static1/");
  location1_statement->child_block_ = std::move(root1_config);

  location2_statement->tokens_.push_back("location");
  location2_statement->tokens_.push_back("/static2/");
  location2_statement->child_block_ = std::move(root2_config);

  handler_statement->tokens_.push_back("handler");
  handler_statement->tokens_.push_back("static");

  server_child_config->statements_.emplace_back(handler_statement);
  server_child_config->statements_.emplace_back(location1_statement);
  server_child_config->statements_.emplace_back(location2_statement);

  server_statement->tokens_.push_back("server");
  server_statement->child_block_ = std::move(server_child_config);

  server_config->statements_.emplace_back(server_statement);

  std::vector<nginx::server> servers = server_config->get_servers();

  EXPECT_EQ(servers.size(), 1);
  EXPECT_EQ(servers[0].handler, "static");
  EXPECT_EQ(servers[0].locations.size(), 2);
  EXPECT_EQ(servers[0].locations[0].value, "/static1/");
  EXPECT_EQ(servers[0].locations[0].root, "./public/public1");
  EXPECT_EQ(servers[0].locations[1].value, "/static2/");
  EXPECT_EQ(servers[0].locations[1].root, "./public/public2");
}

} // namespace nginx