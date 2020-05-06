#include "gtest/gtest.h"
#include "nginx/config_parser.h"
#include "nginx/config.h"

namespace nginx {

class NginxConfigParserTest : public ::testing::Test 
{
protected:
  config_parser parser_;
  config out_config_;
};

TEST_F(NginxConfigParserTest, SimpleConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/example.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, SampleNginxConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/nginx.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, CommentConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/comment.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, EmptyBlockConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/empty_block.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, ConsecutiveClosingBracketsConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/consecutive_closing_brackets.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, NestedBlockConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/nested_block.conf", &out_config_));
}
TEST_F(NginxConfigParserTest, MultipleTokensConfig) 
{
	EXPECT_TRUE(parser_.parse("nginx/configs/multiple_tokens.conf", &out_config_));
}

TEST_F(NginxConfigParserTest, NoTokenConfig) 
{
	EXPECT_FALSE(parser_.parse("nginx/onfigs/no_token.conf", &out_config_));

TEST_F(NginxConfigParserTest, UnmatchedBracketsConfig) 
{
	EXPECT_FALSE(parser_.parse("nginx/configs/unmatched_brackets.conf", &out_config_));
}}
TEST_F(NginxConfigParserTest, NoSemicolonConfig) 
{
	EXPECT_FALSE(parser_.parse("nginx/configs/no_semicolon.conf", &out_config_));
}


} // namespace nginx