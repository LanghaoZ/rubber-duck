#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test {
	protected:
		NginxConfigParser parser;
		NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
	bool success = parser.Parse("configs/example_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, CommentConfig) {
	bool success = parser.Parse("configs/comment_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, UnmatchedBracketsConfig) {
	bool success = parser.Parse("configs/unmatched_brackets_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyBlockConfig) {
	bool success = parser.Parse("configs/empty_block_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, ConsecutiveClosingBracketsConfig) {
	bool success = parser.Parse("configs/consecutive_closing_brackets_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NestedBlockConfig) {
	bool success = parser.Parse("configs/nested_block_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoSemicolonConfig) {
	bool success = parser.Parse("configs/no_semicolon_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MultipleTokensConfig) {
	bool success = parser.Parse("configs/multiple_tokens_config", &out_config);
	EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, NoTokenConfig) {
	bool success = parser.Parse("configs/no_token_config", &out_config);
	EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, GetPortFromNull) {
    bool success = parser.Parse("configs/example_config", &out_config);
    int port = out_config.GetPort(NULL);
    EXPECT_TRUE(port == ERR_NOT_FOUND);
}

TEST_F(NginxConfigParserTest, GetNonNestedPort) {
    bool success = parser.Parse("configs/simple_port_config", &out_config);
    int port = out_config.GetPort(&out_config);
    EXPECT_TRUE(port == 8080);
}

TEST_F(NginxConfigParserTest, GetNestedPort) {
    bool success = parser.Parse("configs/nested_port_config", &out_config);
    int port = out_config.GetPort(&out_config);
    EXPECT_TRUE(port == 5000);
}

TEST_F(NginxConfigParserTest, GetMultiplePort) {
    bool success = parser.Parse("configs/multiple_port_config", &out_config);
    int port = out_config.GetPort(&out_config);
    EXPECT_TRUE(port == 5000);
}