#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"
#include "nginx/config_statement.h"

using ::testing::_;
using ::testing::Return;

class MockConfig : public nginx::config {
public:
  MOCK_METHOD1(to_string, std::string(int));
};

TEST(ConfigStatementTest, EmptyTokensTest) {

  nginx::config_statement statement;
  EXPECT_EQ(statement.to_string(0), ";\n");
}

TEST(ConfigStatementTest, SerializesTokensWithoutChildBlock) {

  nginx::config_statement statement;

  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  statement.tokens_.push_back("foobar");
  EXPECT_EQ(statement.to_string(0), "foo bar foobar;\n");
}

TEST(ConfigStatementTest, SerializesTokensWithChildBlock) {

  nginx::config_statement *statement = new nginx::config_statement;
  statement->tokens_.push_back("foo");
  statement->tokens_.push_back("bar");
  MockConfig * mock_config = new MockConfig;
  EXPECT_CALL(*mock_config, to_string(_)).WillOnce(Return("\tport 8080;\n"));
  statement->child_block_.reset(mock_config);
  EXPECT_EQ(statement->to_string(0), "foo bar {\n\tport 8080;\n}\n");
  delete mock_config;
}

TEST(ConfigStatementTest, SerializesTokensWithOneIndentation) {

  nginx::config_statement statement;

  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  statement.tokens_.push_back("foobar");
  EXPECT_EQ(statement.to_string(1), "  foo bar foobar;\n");
}



