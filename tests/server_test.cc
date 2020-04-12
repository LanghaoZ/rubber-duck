#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "gtest/gtest.h"
#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;

void server_8080_thread()
{
  boost::asio::io_service io_service;
  server s(io_service, 8080);
  io_service.run();
}

TEST(ServerTest, ServerReceivesBadRequest) 
{

  // run the server in a different thread
  boost::thread s(&server_8080_thread);

  // client
  // https://www.boost.org/doc/libs/1_42_0/doc/html/boost_asio/example/http/client/sync_client.cpp
  
  // connect to the server
  boost::asio::io_service c_io_service;
  tcp::socket c_socket(c_io_service);
  tcp::resolver resolver(c_io_service);
  boost::asio::connect(c_socket, resolver.resolve({"localhost", "8080"}));

  // send a message to the server
  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  request_stream << "rubber-duck\r\n";
  
  // Send the request.
  boost::asio::write(c_socket, request);
  
  // Read the response status line.
  boost::asio::streambuf response;
  boost::asio::read_until(c_socket, response, "\r\n");

  // Check that response is bad.
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);

  EXPECT_TRUE(response_stream);
  EXPECT_TRUE(http_version.substr(0, 5) == "HTTP/");
  EXPECT_TRUE(status_code == 400);

}

void server_8081_thread()
{
  boost::asio::io_service io_service;
  server s(io_service, 8081);
  io_service.run();
}

TEST(ServerTest, ServerReceivesValidRequest)
{

  // run the server in a different thread
  boost::thread s(&server_8081_thread);

  // connect to the server
  boost::asio::io_service c_io_service;
  tcp::socket c_socket(c_io_service);
  tcp::resolver resolver(c_io_service);
  boost::asio::connect(c_socket, resolver.resolve({"localhost", "8081"}));

  // send a message to the server
  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  request_stream << "GET / HTTP/1.1\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Accept-Encoding: gzip, deflate\r\n";
  request_stream << "Connection: keep-alive\r\n";
  request_stream << "Host: localhost:8080\r\n";
  request_stream << "User-Agent: HTTPie/0.9.8\r\n";
  request_stream << "Content-Length: 1\r\n";
  request_stream << "\r\n\r\n";
  request_stream << "{\r\n";
  request_stream << "\t\"hello\": \"world\"\r\n";
  request_stream << "}\r\n";

  // Send the request.
  boost::asio::write(c_socket, request);
  
  // Read the response status line.
  boost::asio::streambuf response;
  boost::asio::read_until(c_socket, response, "\r\n");

  // Check that response is ok.
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);

  EXPECT_TRUE(response_stream);
  EXPECT_TRUE(http_version.substr(0, 5) == "HTTP/");
  EXPECT_TRUE(status_code == 200);

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(c_socket, response, "\r\n\r\n");
  
  // Process the response headers.
  bool isContentTypeHeaderFound = false;
  std::string header;
  while (std::getline(response_stream, header) && header != "\r") {

    std::cout << header << std::endl;
    std::string delimiter = ": ";
    std::string fieldName = header.substr(0, header.find(delimiter));
    std::string fieldValue = header.substr(header.find(delimiter) + 2);
    if (fieldName.compare("Content-Type") == 0) {
      EXPECT_TRUE(fieldValue.compare("text/plain\r") == 0);
      isContentTypeHeaderFound = true;
    }
  }
  EXPECT_TRUE(isContentTypeHeaderFound);

  // TODO
  // read response body if exists
  // int numLines = 0;
  // std::string line;

  // EXPECT_TRUE(response.size() > 0);

  // std::getline(response_stream, line);
  // EXPECT_TRUE(line.compare("GET / HTTP/1.1\r") == 0);
  // numLines++;

  // std::cout << numLines << std::endl;

  // while (std::getline(response_stream, line)) numLines++;

  // std::cout << numLines << std::endl;

  // EXPECT_TRUE(numLines == 10);

}