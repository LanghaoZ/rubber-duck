#include "gtest/gtest.h"
#include "http/server/header.h"

TEST(HeaderFieldNameTest, ConvertsAcceptFieldNameTypeToString) {
  EXPECT_STREQ("Accept", http::server::header::field_name_type_as_string(http::server::header::accept));
}

TEST(HeaderFieldNameTest, ConvertsAcceptEncodingFieldNameTypeToString) {
  EXPECT_STREQ("Accept-Encoding", http::server::header::field_name_type_as_string(http::server::header::accept_encoding));
}

TEST(HeaderFieldNameTest, ConvertsConnectionFieldNameTypeToString) {
  EXPECT_STREQ("Connection", http::server::header::field_name_type_as_string(http::server::header::connection));
}

TEST(HeaderFieldNameTest, ConvertsHostFieldNameTypeToString) {
  EXPECT_STREQ("Host", http::server::header::field_name_type_as_string(http::server::header::host));
}

TEST(HeaderFieldNameTest, ConvertsUserAgentFieldNameTypeToString) {
  EXPECT_STREQ("User-Agent", http::server::header::field_name_type_as_string(http::server::header::user_agent));
}

TEST(HeaderFieldNameTest, ConvertsContentLengthFieldNameTypeToString) {
  EXPECT_STREQ("Content-Length", http::server::header::field_name_type_as_string(http::server::header::content_length));
}

TEST(HeaderFieldNameTest, ConvertsContentTypeFieldNameTypeToString) {
  EXPECT_STREQ("Content-Type", http::server::header::field_name_type_as_string(http::server::header::content_type));
}

TEST(HeaderFieldValueTest, ConvertsTextPlainFieldValueTypeToString) {
  EXPECT_STREQ("text/plain", http::server::header::field_value_type_as_string(http::server::header::text_plain));
}