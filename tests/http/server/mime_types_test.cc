#include "gtest/gtest.h"
#include "http/server/mime_types.h"

namespace http {
namespace server {

TEST(MimeTypesTest, Gif) 
{
  EXPECT_EQ(mime_types::extension_to_type("gif"), "image/gif");
}

TEST(MimeTypesTest, Htm) 
{
  EXPECT_EQ(mime_types::extension_to_type("htm"), "text/html");
}

TEST(MimeTypesTest, Html) 
{
  EXPECT_EQ(mime_types::extension_to_type("html"), "text/html");
}

TEST(MimeTypesTest, Jpg) 
{
  EXPECT_EQ(mime_types::extension_to_type("jpg"), "image/jpeg");
}

TEST(MimeTypesTest, Png) 
{
  EXPECT_EQ(mime_types::extension_to_type("png"), "image/png");
}

TEST(MimeTypesTest, Txt) 
{
  EXPECT_EQ(mime_types::extension_to_type("txt"), "text/plain");
}

TEST(MimeTypesTest, Zip) 
{
  EXPECT_EQ(mime_types::extension_to_type("zip"), "application/zip");
}

TEST(MimeTypeTests, TestOtherTypes) 
{
  EXPECT_EQ(mime_types::extension_to_type("foo"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("aaaaaaa"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("tar"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("doc"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("mat"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("h"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type("cc"), "text/plain");
  EXPECT_EQ(mime_types::extension_to_type(""), "text/plain");
}

} // namespace server
} // namespace http