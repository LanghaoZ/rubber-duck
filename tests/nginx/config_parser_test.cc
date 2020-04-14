#include "gtest/gtest.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"

using namespace nginx;

class NginxConfigParserTest : public ::testing::Test {
	protected:
		nginx::config_parser parser;
		nginx::config out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
	bool success = parser.parse("nginx/configs/example_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CommentConfig) {
	bool success = parser.parse("nginx/configs/comment_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, UnmatchedBracketsConfig) {
	bool success = parser.parse("nginx/configs/unmatched_brackets_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyBlockConfig) {
	bool success = parser.parse("nginx/configs/empty_block_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, ConsecutiveClosingBracketsConfig) {
	bool success = parser.parse("nginx/configs/consecutive_closing_brackets_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedBlockConfig) {
	bool success = parser.parse("nginx/configs/nested_block_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoSemicolonConfig) {
	bool success = parser.parse("nginx/configs/no_semicolon_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MultipleTokensConfig) {
	bool success = parser.parse("nginx/configs/multiple_tokens_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoTokenConfig) {
	bool success = parser.parse("nginx/onfigs/no_token_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, GetPortFromNull) {
    bool success = parser.parse("nginx/configs/empty_config", &out_config);
    int port = out_config.get_port();
    EXPECT_TRUE(port == nginx::err_not_found);
}

TEST_F(NginxConfigParserTest, GetNonNestedPort) {
    bool success = parser.parse("nginx/configs/simple_port_config", &out_config);
    int port = out_config.get_port();
    EXPECT_TRUE(port == 8080);
}

TEST_F(NginxConfigParserTest, GetNestedPort) {
    bool success = parser.parse("nginx/configs/nested_port_config", &out_config);
    int port = out_config.get_port();
    EXPECT_TRUE(port == 5000);
}

TEST_F(NginxConfigParserTest, GetMultiplePort) {
    bool success = parser.parse("nginx/configs/multiple_port_config", &out_config);
    int port = out_config.get_port();
    EXPECT_TRUE(port == 5000);
}