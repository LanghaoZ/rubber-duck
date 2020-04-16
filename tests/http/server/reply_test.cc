#include "gtest/gtest.h"
#include "http/server/reply.h"

class ReplyTest : public ::testing::Test {
  protected:
    http::server::reply reply;
    std::string content_length = "Content-Length";
    std::string content_type = "Content-Type";
    std::string text_html = "text/html";

    std::string generate_content(const std::string title, const std::string header)
    {
      return 
        "<html>"
        "<head><title>" + title + "</title></head>"
        "<body><h1>" + header + "</h1></body></html>";
    }
};

TEST_F(ReplyTest, HandlesOkResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::ok);
  EXPECT_EQ(reply.status, http::server::reply::ok);
  EXPECT_EQ(reply.content, "");
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesCreatedResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::created);
  EXPECT_EQ(reply.status, http::server::reply::created);
  EXPECT_EQ(reply.content, generate_content("Created", "201 Created"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesAcceptedResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::accepted);
  EXPECT_EQ(reply.status, http::server::reply::accepted);
  EXPECT_EQ(reply.content, generate_content("Accepted", "202 Accepted"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesNoContentResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::no_content);
  EXPECT_EQ(reply.status, http::server::reply::no_content);
  EXPECT_EQ(reply.content, generate_content("No Content", "204 Content"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesMultipleChoicesResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::multiple_choices);
  EXPECT_EQ(reply.status, http::server::reply::multiple_choices);
  EXPECT_EQ(reply.content, generate_content("Multiple Choices", "300 Multiple Choices"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesMovedPermanentlyResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::moved_permanently);
  EXPECT_EQ(reply.status, http::server::reply::moved_permanently);
  EXPECT_EQ(reply.content, generate_content("Moved Permanently", "301 Moved Permanently"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesMovedTemporarilyResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::moved_temporarily);
  EXPECT_EQ(reply.status, http::server::reply::moved_temporarily);
  EXPECT_EQ(reply.content, generate_content("Moved Temporarily", "302 Moved Temporarily"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesNotModifiedResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::not_modified);
  EXPECT_EQ(reply.status, http::server::reply::not_modified);
  EXPECT_EQ(reply.content, generate_content("Not Modified", "304 Not Modified"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesBadRequestResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::bad_request);
  EXPECT_EQ(reply.status, http::server::reply::bad_request);
  EXPECT_EQ(reply.content, generate_content("Bad Request", "400 Bad Request"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesUnauthorizedResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::unauthorized);
  EXPECT_EQ(reply.status, http::server::reply::unauthorized);
  EXPECT_EQ(reply.content, generate_content("Unauthorized", "401 Unauthorized"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesForbiddenResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::forbidden);
  EXPECT_EQ(reply.status, http::server::reply::forbidden);
  EXPECT_EQ(reply.content, generate_content("Forbidden", "403 Forbidden"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesNotFoundResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::not_found);
  EXPECT_EQ(reply.status, http::server::reply::not_found);
  EXPECT_EQ(reply.content, generate_content("Not Found", "404 Not Found"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesInternalServerErrorResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::internal_server_error);
  EXPECT_EQ(reply.status, http::server::reply::internal_server_error);
  EXPECT_EQ(reply.content, generate_content("Internal Server Error", "500 Internal Server Error"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesNotImplementedResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::not_implemented);
  EXPECT_EQ(reply.status, http::server::reply::not_implemented);
  EXPECT_EQ(reply.content, generate_content("Not Implemented", "501 Not Implemented"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesBadGatewayResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::bad_gateway);
  EXPECT_EQ(reply.status, http::server::reply::bad_gateway);
  EXPECT_EQ(reply.content, generate_content("Bad Gateway", "502 Bad Gateway"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}

TEST_F(ReplyTest, HandlesServiceUnavailableResponse) {
  reply = http::server::reply::stock_reply(http::server::reply::service_unavailable);
  EXPECT_EQ(reply.status, http::server::reply::service_unavailable);
  EXPECT_EQ(reply.content, generate_content("Service Unavailable", "503 Service Unavailable"));
  EXPECT_EQ(reply.headers.size(), 2);
  EXPECT_EQ(reply.headers[0].name, content_length);
  EXPECT_EQ(reply.headers[0].value, std::to_string(reply.content.size()));
  EXPECT_EQ(reply.headers[1].name, content_type);
  EXPECT_EQ(reply.headers[1].value, text_html);
}