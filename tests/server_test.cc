#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "gtest/gtest.h"
#include "server.h"

using boost::asio::ip::tcp;

// void server_thread()
// {
//   boost::asio::io_service s_io_service;

//   server s_socket(s_io_service, 8080);

//   s_io_service.run();
// }

// class ServerTest : public ::testing::Test {
//   protected:
//     void SetUp() override
//     {
//       // run the server in a different thread
//       boost::thread s(&server_thread);
//     }
// };


// TEST_F(ServerTest, ServerEchoesMessageBackToClient) 
// {
//   // client
//   // https://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/example/cpp11/echo/blocking_tcp_echo_client.cpp
//   int max_length = 1024;
//   boost::asio::io_service c_io_service;

//   tcp::socket c_socket(c_io_service);
//   tcp::resolver resolver(c_io_service);
//   boost::asio::connect(c_socket, resolver.resolve({"localhost", "8080"}));

//   // send a message to the server
//   char request[] = "rubber-duck";
//   size_t request_length = std::strlen(request);
//   std::cout << "Request is: ";
//   std::cout.write(request, request_length);
//   std::cout << "\n";
//   boost::asio::write(c_socket, boost::asio::buffer(request, request_length));

//   // receive a message from the server
//   char reply[max_length];
//   size_t reply_length = boost::asio::read(c_socket,
//       boost::asio::buffer(reply, request_length));
//   std::cout << "Reply is: ";
//   std::cout.write(reply, reply_length);
//   std::cout << "\n";
 
//   EXPECT_TRUE(request_length == reply_length);
//   EXPECT_TRUE(std::strncmp(request, reply, request_length) == 0);
// }