#include "gtest/gtest.h"
#include "http/server/header.h"

namespace http {
namespace server {

TEST(HeaderFieldNameTest, ConvertsAcceptFieldNameTypeToString) 
{
  EXPECT_STREQ("Accept", header::field_name_type_as_string(header::accept));
}

TEST(HeaderFieldNameTest, ConvertsAcceptEncodingFieldNameTypeToString) 
{
  EXPECT_STREQ("Accept-Encoding", header::field_name_type_as_string(header::accept_encoding));
}

TEST(HeaderFieldNameTest, ConvertsConnectionFieldNameTypeToString) 
{
  EXPECT_STREQ("Connection", header::field_name_type_as_string(header::connection));
}

TEST(HeaderFieldNameTest, ConvertsHostFieldNameTypeToString) 
{
  EXPECT_STREQ("Host", header::field_name_type_as_string(header::host));
}

TEST(HeaderFieldNameTest, ConvertsUserAgentFieldNameTypeToString) 
{
  EXPECT_STREQ("User-Agent", header::field_name_type_as_string(header::user_agent));
}

TEST(HeaderFieldNameTest, ConvertsContentLengthFieldNameTypeToString) 
{
  EXPECT_STREQ("Content-Length", header::field_name_type_as_string(header::content_length));
}

TEST(HeaderFieldNameTest, ConvertsContentTypeFieldNameTypeToString) 
{
  EXPECT_STREQ("Content-Type", header::field_name_type_as_string(header::content_type));
}

TEST(HeaderFieldValueTest, ConvertsTextPlainFieldValueTypeToString) 
{
  EXPECT_STREQ("text/plain", header::field_value_type_as_string(header::text_plain));
}

} // namespace server
} // namespace http