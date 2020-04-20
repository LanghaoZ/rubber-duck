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

TEST(ConfigTest, EmptyConfigTest){
	nginx::config * config = new nginx::config;
	EXPECT_EQ(config->to_string(0), "");
	delete config;
}

TEST(ConfigTest, ToStringTest){
	nginx::config * config = new nginx::config;
	MockConfigStatment * mock_config_statment = new MockConfigStatment;
	config->statements_.emplace_back(mock_config_statment);
	EXPECT_CALL(*mock_config_statment, to_string(_)).WillOnce(Return("Server;"));
	EXPECT_EQ(config->to_string(0), "Server;");
	delete config;
}

TEST(ConfigTest, GetPortTestWithPort8080){
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

TEST(ConfigTest, GetPortTestWithoutPort){
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

