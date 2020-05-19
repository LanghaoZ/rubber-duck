#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nginx/config.h"
#include "nginx/config_parser.h"
#include "nginx/config_statement.h"
#include "nginx/location.h"

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
  std::shared_ptr<mock_config_statement> conf_statement = std::make_shared<mock_config_statement>();
	EXPECT_CALL(*conf_statement, to_string(_)).WillOnce(Return("server;"));
  
  std::unique_ptr<config> conf = std::make_unique<config>();
	conf->statements_.emplace_back(conf_statement);
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
	std::shared_ptr<config_statement> statement_foo_bar = std::make_shared<config_statement>();
  statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");

  std::shared_ptr<config_statement> statement_port_8080 = std::make_shared<config_statement>();
  statement_port_8080->tokens_.push_back("port");
	statement_port_8080->tokens_.push_back("8080");

  std::unique_ptr<config> inner_config = std::make_unique<config>();
	inner_config->statements_.emplace_back(statement_foo_bar);
	inner_config->statements_.emplace_back(statement_port_8080);

  std::shared_ptr<config_statement> server_statement = std::make_shared<config_statement>();
	server_statement->tokens_.push_back("server");
	server_statement->child_block_ = std::move(inner_config);

  std::unique_ptr<config> outer_config = std::make_unique<config>();
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

	std::shared_ptr<config_statement> statement_foo_bar = std::make_shared<config_statement>();
	statement_foo_bar->tokens_.push_back("foo");
	statement_foo_bar->tokens_.push_back("bar");
  
  std::unique_ptr<config> inner_config = std::make_unique<config>();
	inner_config->statements_.emplace_back(statement_foo_bar);
  
  std::shared_ptr<config_statement> server_statement = std::make_shared<config_statement>();
	server_statement->tokens_.push_back("server");
	server_statement->child_block_ = std::move(inner_config);

  std::unique_ptr<config> outer_config = std::make_unique<config>();
	outer_config->statements_.emplace_back(server_statement);

	EXPECT_EQ(outer_config->get_port(), -1);
}

TEST(ConfigTest, GetLocationsTest) 
{
  /**
   * location /static1/ static_handler {
   *   root ./public/public1;
   * }
   * location /static2/ static_handler {
   *   root ./public/public2;
   * }
   */
  std::unique_ptr<config> server_config = std::make_unique<config>();
  std::unique_ptr<config> root1_config = std::make_unique<config>();
  std::unique_ptr<config> root2_config = std::make_unique<config>();
  std::shared_ptr<config_statement> root1_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> root2_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> location1_statement = std::make_shared<config_statement>();
  std::shared_ptr<config_statement> location2_statement = std::make_shared<config_statement>();
  
  root1_statement->tokens_.push_back("root");
  root1_statement->tokens_.push_back("./public/public1");
  
  root2_statement->tokens_.push_back("root");
  root2_statement->tokens_.push_back("./public/public2");

  root1_config->statements_.emplace_back(root1_statement);
  root2_config->statements_.emplace_back(root2_statement);

  location1_statement->tokens_.push_back("location");
  location1_statement->tokens_.push_back("/static1/");
  location1_statement->tokens_.push_back("static_handler");
  location1_statement->child_block_ = std::move(root1_config);

  location2_statement->tokens_.push_back("location");
  location2_statement->tokens_.push_back("/static2/");
  location2_statement->tokens_.push_back("static_handler");
  location2_statement->child_block_ = std::move(root2_config);

  server_config->statements_.emplace_back(location1_statement);
  server_config->statements_.emplace_back(location2_statement);

  std::vector<nginx::location> locations = server_config->get_locations();

  EXPECT_EQ(locations.size(), 2);
  EXPECT_EQ(locations[0].path, "/static1/");
  EXPECT_EQ(locations[0].handler, "static_handler");
  EXPECT_EQ(locations[1].path, "/static2/");
  EXPECT_EQ(locations[1].handler, "static_handler");
}

} // namespace nginx