#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"

using ::testing::_;
using ::testing::Return;

namespace nginx {

class mock_config : public config 
{
public:
  MOCK_METHOD1(to_string, std::string(int));
};

TEST(ConfigStatementTest, EmptyTokensTest) 
{
  config_statement statement;
  EXPECT_EQ(statement.to_string(0), ";\n");
}

TEST(ConfigStatementTest, SerializesTokensWithoutChildBlock) 
{
  /**
   * foo bar foobar; 
   */
  config_statement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  statement.tokens_.push_back("foobar");
  EXPECT_EQ(statement.to_string(0), "foo bar foobar;\n");
}

TEST(ConfigStatementTest, SerializesTokensWithChildBlock) 
{
  /**
   * foo bar {
   *   port 8080;
   * }
   */ 
  config_statement statement;
  std::unique_ptr<mock_config> conf = std::make_unique<mock_config>();

  EXPECT_CALL(*conf, to_string(_)).WillOnce(Return("  port 8080;\n"));
  
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  statement.child_block_ = std::move(conf);

  EXPECT_EQ(statement.to_string(0), "foo bar {\n  port 8080;\n}\n");
}

TEST(ConfigStatementTest, SerializesTokensWithOneIndentation) 
{
  /**
   *   foo bar foobar; 
   */
  config_statement statement;

  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  statement.tokens_.push_back("foobar");

  EXPECT_EQ(statement.to_string(1), "  foo bar foobar;\n");
}

} // namespace nginx