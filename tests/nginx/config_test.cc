#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nginx/config.h"
#include "nginx/config_parser.h"
#include "nginx/config_statement.h"

using ::testing::_;
using ::testing::Return;

class MockConfigStatment : public nginx::config_statement {
public:
  MOCK_METHOD1(to_string, std::string(int));
};

TEST(ConfigTest, EmptyConfigTest) {
	nginx::config * config = new nginx::config;
	EXPECT_EQ(config->to_string(0), "");
	delete config;
}

TEST(ConfigTest, ToStringTest) {
	nginx::config * config = new nginx::config;
	MockConfigStatment * mock_config_statment = new MockConfigStatment;
	config->statements_.emplace_back(mock_config_statment);
	EXPECT_CALL(*mock_config_statment, to_string(_)).WillOnce(Return("Server;"));
	EXPECT_EQ(config->to_string(0), "Server;");
	delete config;
}

TEST(ConfigTest, GetPortTestWithPort8080) {
	nginx::config * inner_config = new nginx::config;
	nginx::config_statement * statement_foo_bar = new nginx::config_statement;
	nginx::config_statement * statement_port_8080 = new nginx::config_statement;
	statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");
	statement_port_8080->tokens_.push_back("port");
	statement_port_8080->tokens_.push_back("8080");
	inner_config->statements_.emplace_back(statement_foo_bar);
	inner_config->statements_.emplace_back(statement_port_8080);
	nginx::config_statement * server_statement = new nginx::config_statement;
	server_statement->tokens_.push_back("Server");
	server_statement->child_block_.reset(inner_config);
	nginx::config * out_config = new nginx::config;
	out_config->statements_.emplace_back(server_statement);
	EXPECT_EQ(out_config->get_port(), 8080);
	delete out_config;
}

TEST(ConfigTest, GetPortTestWithoutPort) {
	nginx::config * inner_config = new nginx::config;
	nginx::config_statement * statement_foo_bar = new nginx::config_statement;
	statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");
	inner_config->statements_.emplace_back(statement_foo_bar);
	nginx::config_statement * server_statement = new nginx::config_statement;
	server_statement->tokens_.push_back("Server");
	server_statement->child_block_.reset(inner_config);
	nginx::config * out_config = new nginx::config;
	out_config->statements_.emplace_back(server_statement);
	EXPECT_EQ(out_config->get_port(), -1);
	delete out_config;
}

TEST(ConfigTest, GetServersTest) {
  std::shared_ptr<nginx::config_statement> root1_statement
    = std::make_shared<nginx::config_statement>();
  root1_statement.get()->tokens_.push_back("root");
  root1_statement.get()->tokens_.push_back("./public/public1");

  std::shared_ptr<nginx::config_statement> root2_statement
    = std::make_shared<nginx::config_statement>();
  root2_statement.get()->tokens_.push_back("root");
  root2_statement.get()->tokens_.push_back("./public/public2");

  nginx::config* root1_config = new nginx::config;
  root1_config->statements_.emplace_back(root1_statement);

  nginx::config* root2_config = new nginx::config;
  root2_config->statements_.emplace_back(root2_statement);

  std::shared_ptr<nginx::config_statement> location1_statement
    = std::make_shared<nginx::config_statement>();
  location1_statement.get()->tokens_.push_back("location");
  location1_statement.get()->tokens_.push_back("/static1/");
  location1_statement.get()->child_block_.reset(root1_config);

  std::shared_ptr<nginx::config_statement> location2_statement
    = std::make_shared<nginx::config_statement>();
  location2_statement.get()->tokens_.push_back("location");
  location2_statement.get()->tokens_.push_back("/static2/");
  location2_statement.get()->child_block_.reset(root2_config);

  std::shared_ptr<nginx::config_statement> handler_statement
    = std::make_shared<nginx::config_statement>();
  handler_statement.get()->tokens_.push_back("handler");
  handler_statement.get()->tokens_.push_back("static");

  nginx::config* server_child_config = new nginx::config;
  server_child_config->statements_.emplace_back(handler_statement);
  server_child_config->statements_.emplace_back(location1_statement);
  server_child_config->statements_.emplace_back(location2_statement);

  std::shared_ptr<nginx::config_statement> server_statement
    = std::make_shared<nginx::config_statement>();
  server_statement.get()->tokens_.push_back("server");
  server_statement.get()->child_block_.reset(server_child_config);

  std::unique_ptr<nginx::config> server_config = std::make_unique<nginx::config>();
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