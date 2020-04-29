#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/mime_types.h"

class MimeTypeTests : public ::testing::Test 
{
  protected:
    std::string extension;
	  std::string mimetype;
};

TEST(MimeTypesTest, Gif) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("gif"), "image/gif");
}

TEST(MimeTypesTest, Htm) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("htm"), "text/html");
}

TEST(MimeTypesTest, Html) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("html"), "text/html");
}

TEST(MimeTypesTest, Jpg) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("jpg"), "image/jpeg");
}

TEST(MimeTypesTest, Png) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("png"), "image/png");
}

TEST(MimeTypesTest, Txt) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("txt"), "text/plain");
}

TEST(MimeTypesTest, Zip) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("zip"), "application/zip");
}

TEST(MimeTypesTest, Foo) {
  EXPECT_EQ(http::server::mime_types::extension_to_type("foo"), "text/plain");
}

TEST_F(MimeTypeTests, TestOtherTypes) 
{
  bool success = 1;
  
  extension = "aaaaaaa";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  EXPECT_TRUE(success);
  
  extension = "tar";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  
  extension = "doc";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  
  extension = "mat";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  
  extension = "h";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  
  extension = "cc";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");

  extension = "";
  mimetype = http::server::mime_types::extension_to_type(extension);
  success = success && (mimetype == "text/plain");
  
  EXPECT_TRUE(success);
}
