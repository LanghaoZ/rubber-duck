#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "http/response.h"
#include "http/status_code.h"

namespace http {

class ResponseTest : public ::testing::Test 
{
protected:
  response res_;
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

TEST_F(ResponseTest, HandlesOkResponse) 
{
  res_ = status_code_to_stock_response(status_code::ok);
  EXPECT_EQ(res_.code, status_code::ok);
  EXPECT_EQ(res_.body, "");
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesCreatedResponse)
{
  res_ = status_code_to_stock_response(status_code::created);
  EXPECT_EQ(res_.code, status_code::created);
  EXPECT_EQ(res_.body, generate_content("Created", "201 Created"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesAcceptedResponse) 
{
  res_ = status_code_to_stock_response(status_code::accepted);
  EXPECT_EQ(res_.code, status_code::accepted);
  EXPECT_EQ(res_.body, generate_content("Accepted", "202 Accepted"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesNoContentResponse) 
{
  res_ = status_code_to_stock_response(status_code::no_content);
  EXPECT_EQ(res_.code, status_code::no_content);
  EXPECT_EQ(res_.body, generate_content("No Content", "204 Content"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesMultipleChoicesResponse) 
{
  res_ = status_code_to_stock_response(status_code::multiple_choices);
  EXPECT_EQ(res_.code, status_code::multiple_choices);
  EXPECT_EQ(res_.body, generate_content("Multiple Choices", "300 Multiple Choices"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesMovedPermanentlyResponse) 
{
  res_ = status_code_to_stock_response(status_code::moved_permanently);
  EXPECT_EQ(res_.code, status_code::moved_permanently);
  EXPECT_EQ(res_.body, generate_content("Moved Permanently", "301 Moved Permanently"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesMovedTemporarilyResponse) 
{
  res_ = status_code_to_stock_response(status_code::moved_temporarily);
  EXPECT_EQ(res_.code, status_code::moved_temporarily);
  EXPECT_EQ(res_.body, generate_content("Moved Temporarily", "302 Moved Temporarily"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesNotModifiedResponse) 
{
  res_ = status_code_to_stock_response(status_code::not_modified);
  EXPECT_EQ(res_.code, status_code::not_modified);
  EXPECT_EQ(res_.body, generate_content("Not Modified", "304 Not Modified"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesBadRequestResponse) 
{
  res_ = status_code_to_stock_response(status_code::bad_request);
  EXPECT_EQ(res_.code, status_code::bad_request);
  EXPECT_EQ(res_.body, generate_content("Bad Request", "400 Bad Request"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesUnauthorizedResponse) 
{
  res_ = status_code_to_stock_response(status_code::unauthorized);
  EXPECT_EQ(res_.code, status_code::unauthorized);
  EXPECT_EQ(res_.body, generate_content("Unauthorized", "401 Unauthorized"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesForbiddenResponse) 
{
  res_ = status_code_to_stock_response(status_code::forbidden);
  EXPECT_EQ(res_.code, status_code::forbidden);
  EXPECT_EQ(res_.body, generate_content("Forbidden", "403 Forbidden"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesNotFoundResponse) 
{
  res_ = status_code_to_stock_response(status_code::not_found);
  EXPECT_EQ(res_.code, status_code::not_found);
  EXPECT_EQ(res_.body, generate_content("Not Found", "404 Not Found"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesInternalServerErrorResponse) 
{
  res_ = status_code_to_stock_response(status_code::internal_server_error);
  EXPECT_EQ(res_.code, status_code::internal_server_error);
  EXPECT_EQ(res_.body, generate_content("Internal Server Error", "500 Internal Server Error"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesNotImplementedResponse) 
{
  res_ = status_code_to_stock_response(status_code::not_implemented);
  EXPECT_EQ(res_.code, status_code::not_implemented);
  EXPECT_EQ(res_.body, generate_content("Not Implemented", "501 Not Implemented"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesBadGatewayResponse) 
{
  res_ = status_code_to_stock_response(status_code::bad_gateway);
  EXPECT_EQ(res_.code, status_code::bad_gateway);
  EXPECT_EQ(res_.body, generate_content("Bad Gateway", "502 Bad Gateway"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesServiceUnavailableResponse)
 {
  res_ = status_code_to_stock_response(status_code::service_unavailable);
  EXPECT_EQ(res_.code, status_code::service_unavailable);
  EXPECT_EQ(res_.body, generate_content("Service Unavailable", "503 Service Unavailable"));
  EXPECT_EQ(res_.headers[content_length_], std::to_string(res_.body.size()));
  EXPECT_EQ(res_.headers[content_type_], text_html_);
}

TEST_F(ResponseTest, HandlesConvertingOkReplyToBuffer) 
{
  res_.code = status_code::ok;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 200 OK\r\n");
}

TEST_F(ResponseTest, HandlesConvertingCreatedReplyToBuffer) 
{
  res_.code = status_code::created;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 201 Created\r\n");
}

TEST_F(ResponseTest, HandlesConvertingAcceptedReplyToBuffer) 
{
  res_.code = status_code::accepted;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 202 Accepted\r\n");
}

TEST_F(ResponseTest, HandlesConvertingNoContentReplyToBuffer) 
{
  res_.code = status_code::no_content;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 204 No Content\r\n");
}

TEST_F(ResponseTest, HandlesConvertingMultipleChoicesReplyToBuffer) 
{
  res_.code = status_code::multiple_choices;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 300 Multiple Choices\r\n");
}

TEST_F(ResponseTest, HandlesConvertingMovedPermanentlyReplyToBuffer) 
{
  res_.code = status_code::moved_permanently;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 301 Moved Permanently\r\n");
}

TEST_F(ResponseTest, HandlesConvertingMovedTemporarilyReplyToBuffer) 
{
  res_.code = status_code::moved_temporarily;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 302 Moved Temporarily\r\n");
}

TEST_F(ResponseTest, HandlesConvertingNotModifiedReplyToBuffer) 
{
  res_.code = status_code::not_modified;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 304 Not Modified\r\n");
}

TEST_F(ResponseTest, HandlesConvertingBadRequestReplyToBuffer)
 {
  res_.code = status_code::bad_request;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 400 Bad Request\r\n");
}

TEST_F(ResponseTest, HandlesConvertingUnauthorizedReplyToBuffer) 
{
  res_.code = status_code::unauthorized;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 401 Unauthorized\r\n");
}

TEST_F(ResponseTest, HandlesConvertingForbiddenReplyToBuffer) 
{
  res_.code = status_code::forbidden;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 403 Forbidden\r\n");
}

TEST_F(ResponseTest, HandlesConvertingNotFoundReplyToBuffer) 
{
  res_.code = status_code::not_found;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 404 Not Found\r\n");
}

TEST_F(ResponseTest, HandlesConvertingInternalServerErrorReplyToBuffer) 
{
  res_.code = status_code::internal_server_error;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 500 Internal Server Error\r\n");
}

TEST_F(ResponseTest, HandlesConvertingNotImplementedReplyToBuffer) 
{
  res_.code = status_code::not_implemented;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 501 Not Implemented\r\n");
}

TEST_F(ResponseTest, HandlesConvertingBadGatewayReplyToBuffer) 
{
  res_.code = status_code::bad_gateway;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 502 Bad Gateway\r\n");
}

TEST_F(ResponseTest, HandlesConvertingServiceUnavailableReplyToBuffer) 
{
  res_.code = status_code::service_unavailable;
  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 503 Service Unavailable\r\n");
}

TEST_F(ResponseTest, HandlesConvertingReplyToBuffers) 
{
  res_.code = status_code::ok;
  res_.headers["foo"] = "bar";

  std::vector<boost::asio::const_buffer> buffers = response_to_buffers(res_);
  
  EXPECT_EQ(buffers.size(), 7);
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[0]), boost::asio::buffer_size(buffers[0])), "HTTP/1.1 200 OK\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[1]), boost::asio::buffer_size(buffers[1])), "foo");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[2]), boost::asio::buffer_size(buffers[2])), ": ");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[3]), boost::asio::buffer_size(buffers[3])), "bar");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[4]), boost::asio::buffer_size(buffers[4])), "\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[5]), boost::asio::buffer_size(buffers[5])), "\r\n");
  EXPECT_EQ(std::string(boost::asio::buffer_cast<const char*>(buffers[6]), boost::asio::buffer_size(buffers[6])), "");
}

} // namespace http