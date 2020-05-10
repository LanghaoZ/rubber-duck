#include "gtest/gtest.h"
#include "http/server/header.h"

namespace http {
namespace server {

TEST(HeaderFieldNameTest, ConvertsAcceptFieldNameTypeToString) 
{
  EXPECT_EQ("Accept", header::field_name_type_as_string(header::accept));
}

TEST(HeaderFieldNameTest, ConvertsAcceptEncodingFieldNameTypeToString) 
{
  EXPECT_EQ("Accept-Encoding", header::field_name_type_as_string(header::accept_encoding));
}

TEST(HeaderFieldNameTest, ConvertsConnectionFieldNameTypeToString) 
{
  EXPECT_EQ("Connection", header::field_name_type_as_string(header::connection));
}

TEST(HeaderFieldNameTest, ConvertsHostFieldNameTypeToString) 
{
  EXPECT_EQ("Host", header::field_name_type_as_string(header::host));
}

TEST(HeaderFieldNameTest, ConvertsUserAgentFieldNameTypeToString) 
{
  EXPECT_EQ("User-Agent", header::field_name_type_as_string(header::user_agent));
}

TEST(HeaderFieldNameTest, ConvertsContentLengthFieldNameTypeToString) 
{
  EXPECT_EQ("Content-Length", header::field_name_type_as_string(header::content_length));
}

TEST(HeaderFieldNameTest, ConvertsContentTypeFieldNameTypeToString) 
{
  EXPECT_EQ("Content-Type", header::field_name_type_as_string(header::content_type));
}

TEST(HeaderFieldValueTest, ConvertsTextPlainFieldValueTypeToString) 
{
  EXPECT_EQ("text/plain", header::field_value_type_as_string(header::text_plain));
}

} // namespace server
} // namespace http