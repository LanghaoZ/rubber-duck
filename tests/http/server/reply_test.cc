#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/server/reply.h"

namespace http {
namespace server {

class ReplyTest : public ::testing::Test 
{
protected:
  reply reply_;
  const std::string content_length_ = "Content-Length";
  const std::string content_type_ = "Content-Type";
  const std::string text_html_ = "text/html";

  std::string generate_content(const std::string title, const std::string header)
  {
    return 
      "<html>"
      "<head><title>" + title + "</title></head>"
      "<body><h1>" + header + "</h1></body></html>";
  }
};

TEST_F(ReplyTest, HandlesOkResponse) 
{
  reply_ = reply::stock_reply(reply::ok);
  EXPECT_EQ(reply_.status, reply::ok);
  EXPECT_EQ(reply_.content, "");
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesCreatedResponse)
{
  reply_ = reply::stock_reply(reply::created);
  EXPECT_EQ(reply_.status, reply::created);
  EXPECT_EQ(reply_.content, generate_content("Created", "201 Created"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesAcceptedResponse) 
{
  reply_ = reply::stock_reply(reply::accepted);
  EXPECT_EQ(reply_.status, reply::accepted);
  EXPECT_EQ(reply_.content, generate_content("Accepted", "202 Accepted"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesNoContentResponse) 
{
  reply_ = reply::stock_reply(reply::no_content);
  EXPECT_EQ(reply_.status, reply::no_content);
  EXPECT_EQ(reply_.content, generate_content("No Content", "204 Content"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesMultipleChoicesResponse) 
{
  reply_ = reply::stock_reply(reply::multiple_choices);
  EXPECT_EQ(reply_.status, reply::multiple_choices);
  EXPECT_EQ(reply_.content, generate_content("Multiple Choices", "300 Multiple Choices"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesMovedPermanentlyResponse) 
{
  reply_ = reply::stock_reply(reply::moved_permanently);
  EXPECT_EQ(reply_.status, reply::moved_permanently);
  EXPECT_EQ(reply_.content, generate_content("Moved Permanently", "301 Moved Permanently"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesMovedTemporarilyResponse) 
{
  reply_ = reply::stock_reply(reply::moved_temporarily);
  EXPECT_EQ(reply_.status, reply::moved_temporarily);
  EXPECT_EQ(reply_.content, generate_content("Moved Temporarily", "302 Moved Temporarily"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesNotModifiedResponse) 
{
  reply_ = reply::stock_reply(reply::not_modified);
  EXPECT_EQ(reply_.status, reply::not_modified);
  EXPECT_EQ(reply_.content, generate_content("Not Modified", "304 Not Modified"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesBadRequestResponse) 
{
  reply_ = reply::stock_reply(reply::bad_request);
  EXPECT_EQ(reply_.status, reply::bad_request);
  EXPECT_EQ(reply_.content, generate_content("Bad Request", "400 Bad Request"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesUnauthorizedResponse) 
{
  reply_ = reply::stock_reply(reply::unauthorized);
  EXPECT_EQ(reply_.status, reply::unauthorized);
  EXPECT_EQ(reply_.content, generate_content("Unauthorized", "401 Unauthorized"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesForbiddenResponse) 
{
  reply_ = reply::stock_reply(reply::forbidden);
  EXPECT_EQ(reply_.status, reply::forbidden);
  EXPECT_EQ(reply_.content, generate_content("Forbidden", "403 Forbidden"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesNotFoundResponse) 
{
  reply_ = reply::stock_reply(reply::not_found);
  EXPECT_EQ(reply_.status, reply::not_found);
  EXPECT_EQ(reply_.content, generate_content("Not Found", "404 Not Found"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesInternalServerErrorResponse) 
{
  reply_ = reply::stock_reply(reply::internal_server_error);
  EXPECT_EQ(reply_.status, reply::internal_server_error);
  EXPECT_EQ(reply_.content, generate_content("Internal Server Error", "500 Internal Server Error"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesNotImplementedResponse) 
{
  reply_ = reply::stock_reply(reply::not_implemented);
  EXPECT_EQ(reply_.status, reply::not_implemented);
  EXPECT_EQ(reply_.content, generate_content("Not Implemented", "501 Not Implemented"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesBadGatewayResponse) 
{
  reply_ = reply::stock_reply(reply::bad_gateway);
  EXPECT_EQ(reply_.status, reply::bad_gateway);
  EXPECT_EQ(reply_.content, generate_content("Bad Gateway", "502 Bad Gateway"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesServiceUnavailableResponse)
 {
  reply_ = reply::stock_reply(reply::service_unavailable);
  EXPECT_EQ(reply_.status, reply::service_unavailable);
  EXPECT_EQ(reply_.content, generate_content("Service Unavailable", "503 Service Unavailable"));
  EXPECT_EQ(reply_.headers.size(), 2);
  EXPECT_EQ(reply_.headers[0].name, content_length_);
  EXPECT_EQ(reply_.headers[0].value, std::to_string(reply_.content.size()));
  EXPECT_EQ(reply_.headers[1].name, content_type_);
  EXPECT_EQ(reply_.headers[1].value, text_html_);
}

TEST_F(ReplyTest, HandlesConvertingOkReplyToBuffer) 
{
  reply_.status = reply::ok;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 200 OK\r\n");
}

TEST_F(ReplyTest, HandlesConvertingCreatedReplyToBuffer) 
{
  reply_.status = reply::created;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 201 Created\r\n");
}

TEST_F(ReplyTest, HandlesConvertingAcceptedReplyToBuffer) 
{
  reply_.status = reply::accepted;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 202 Accepted\r\n");
}

TEST_F(ReplyTest, HandlesConvertingNoContentReplyToBuffer) 
{
  reply_.status = reply::no_content;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 204 No Content\r\n");
}

TEST_F(ReplyTest, HandlesConvertingMultipleChoicesReplyToBuffer) 
{
  reply_.status = reply::multiple_choices;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 300 Multiple Choices\r\n");
}

TEST_F(ReplyTest, HandlesConvertingMovedPermanentlyReplyToBuffer) 
{
  reply_.status = reply::moved_permanently;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 301 Moved Permanently\r\n");
}

TEST_F(ReplyTest, HandlesConvertingMovedTemporarilyReplyToBuffer) 
{
  reply_.status = reply::moved_temporarily;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 302 Moved Temporarily\r\n");
}

TEST_F(ReplyTest, HandlesConvertingNotModifiedReplyToBuffer) 
{
  reply_.status = reply::not_modified;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 304 Not Modified\r\n");
}

TEST_F(ReplyTest, HandlesConvertingBadRequestReplyToBuffer)
 {
  reply_.status = reply::bad_request;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 400 Bad Request\r\n");
}

TEST_F(ReplyTest, HandlesConvertingUnauthorizedReplyToBuffer) 
{
  reply_.status = reply::unauthorized;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 401 Unauthorized\r\n");
}

TEST_F(ReplyTest, HandlesConvertingForbiddenReplyToBuffer) 
{
  reply_.status = reply::forbidden;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 403 Forbidden\r\n");
}

TEST_F(ReplyTest, HandlesConvertingNotFoundReplyToBuffer) 
{
  reply_.status = reply::not_found;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 404 Not Found\r\n");
}

TEST_F(ReplyTest, HandlesConvertingInternalServerErrorReplyToBuffer) 
{
  reply_.status = reply::internal_server_error;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 500 Internal Server Error\r\n");
}

TEST_F(ReplyTest, HandlesConvertingNotImplementedReplyToBuffer) 
{
  reply_.status = reply::not_implemented;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 501 Not Implemented\r\n");
}

TEST_F(ReplyTest, HandlesConvertingBadGatewayReplyToBuffer) 
{
  reply_.status = reply::bad_gateway;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 502 Bad Gateway\r\n");
}

TEST_F(ReplyTest, HandlesConvertingServiceUnavailableReplyToBuffer) 
{
  reply_.status = reply::service_unavailable;
  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 503 Service Unavailable\r\n");
}

TEST_F(ReplyTest, HandlesConvertingReplyToBuffers) 
{
  reply_.status = reply::ok;
  reply_.headers.resize(1);
  reply_.headers[0].name = "foo";
  reply_.headers[0].value = "bar";

  std::vector<boost::asio::const_buffer> buffers = reply_.to_buffers();
  
  EXPECT_EQ(buffers.size(), 7);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 200 OK\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[1]), boost::asio::buffer_size(buffers[1])), "foo");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[2]), boost::asio::buffer_size(buffers[2])), ": ");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[3]), boost::asio::buffer_size(buffers[3])), "bar");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[4]), boost::asio::buffer_size(buffers[4])), "\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[5]), boost::asio::buffer_size(buffers[5])), "\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[6]), boost::asio::buffer_size(buffers[6])), "");
}

} // namespace server
} // namespace http